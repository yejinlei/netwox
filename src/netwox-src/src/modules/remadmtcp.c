/*
                                  NETWOX
                             Network toolbox
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwox.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----

  This file is part of Netwox.

  Netwox is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwox is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

/*-------------------------------------------------------------*/
#include "../netwox.h"

/*-------------------------------------------------------------*/
#define NETWOX_REMADMTCP_KEY_LEN 1024

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_io_read(netwox_remadmtcp_io *pbio)
{
  netwib_time endtime;
  netwib_bool event;
  netwib_uint32 previoussize;
  netwib_err ret;

  while(NETWIB_TRUE) {
    /* if data contains a valid TLV, exit */
    ret = netwib_tlv_entry_typelen(&pbio->bufrd, NULL, NULL, NULL);
    if (ret == NETWIB_ERR_OK) return(ret);
    /* wait for more data */
    netwib_er(netwib_time_init_now(&endtime));
    netwib_er(netwib_time_plus_sec(&endtime, pbio->timeoutsec));
    netwib_er(netwib_io_wait_read(pbio->pio, &endtime, &event));
    if (!event) {
      return(NETWOX_ERR_TIMEOUT);
    }
    /* save previous size */
    previoussize = netwib__buf_ref_data_size(&pbio->bufrd);
    /* read */
    ret = netwib_io_read(pbio->pio, &pbio->bufrd);
    if (ret == NETWIB_ERR_OK) {
      /* eventually decypher */
      if (pbio->cypher) {
        pbio->bufrd.beginoffset += previoussize;
        netwib_er(netwox_xor_decypher(&pbio->xorrd, &pbio->bufrd));
        pbio->bufrd.beginoffset -= previoussize;
      }
    } else if (ret != NETWIB_ERR_DATANOTAVAIL) {
      return(ret);
    }
    ret = NETWIB_ERR_OK;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_io_write(netwib_bufpool *pbufpool,
                                            netwox_remadmtcp_io *pbio,
                                            netwib_constbuf *pbuf)
{
  netwib_buf *pxorbuf;

  if (pbio->cypher) {
    netwib_er(netwib_bufpool_buf_init(pbufpool, &pxorbuf));
    netwib_er(netwib_buf_append_buf(pbuf, pxorbuf));
    netwib_er(netwox_xor_cypher(&pbio->xorwr, pxorbuf));
    netwib_er(netwib_io_write(pbio->pio, pxorbuf));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &pxorbuf));
  } else {
    netwib_er(netwib_io_write(pbio->pio, pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_io_read_uint32(netwox_remadmtcp_io *pbio,
                                                  netwib_uint32 *pui)
{
  netwib_uint32 skipsize;

  netwib_er(netwox_remadmtcp_io_read(pbio));

  netwib_er(netwib_tlv_decode_uint32(&pbio->bufrd, pui, &skipsize));
  pbio->bufrd.beginoffset += skipsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_io_write_uint32(netwib_bufpool *pbufpool,
                                                   netwox_remadmtcp_io *pbio,
                                                   netwib_uint32 ui)
{
  netwib_buf *ptlvbuf;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &ptlvbuf));
  netwib_er(netwib_tlv_append_uint32(ui, ptlvbuf));
  netwib_er(netwox_remadmtcp_io_write(pbufpool, pbio, ptlvbuf));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &ptlvbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_io_read_buf(netwox_remadmtcp_io *pbio,
                                               netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_uint32 skipsize;

  netwib_er(netwox_remadmtcp_io_read(pbio));

  netwib_er(netwib_tlv_decode_buf(&pbio->bufrd, &buf, &skipsize));
  netwib_er(netwib_buf_append_buf(&buf, pbuf));
  pbio->bufrd.beginoffset += skipsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_io_write_buf(netwib_bufpool *pbufpool,
                                                netwox_remadmtcp_io *pbio,
                                                netwib_constbuf *pbuf)
{
  netwib_buf *ptlvbuf;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &ptlvbuf));
  netwib_er(netwib_tlv_append_buf(pbuf, ptlvbuf));
  netwib_er(netwox_remadmtcp_io_write(pbufpool, pbio, ptlvbuf));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &ptlvbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_io_read_io(netwib_bufpool *pbufpool,
                                              netwox_remadmtcp_io *pbio,
                                              netwib_io *pio)
{
  netwib_buf *pbuf;
  netwib_err ret, ret2;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    /* read chunk */
    netwib__buf_reinit(pbuf);
    ret = netwox_remadmtcp_io_read_buf(pbio, pbuf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (netwib__buf_ref_data_size(pbuf) == 0) {
      /* last part is error code */
      ret = netwox_remadmtcp_io_read_uint32(pbio, (netwib_uint32*)&ret2);
      if (ret == NETWIB_ERR_OK) {
        ret = ret2;
      }
      break;
    }
    /* write to pio */
    ret = netwib_io_write(pio, pbuf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_io_write_io(netwib_bufpool *pbufpool,
                                               netwox_remadmtcp_io *pbio,
                                               netwib_io *pio)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    /* read chunk */
    netwib__buf_reinit(pbuf);
    ret = netwib_io_read(pio, pbuf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (netwib__buf_ref_data_size(pbuf) == 0) {
      continue;
    }
    /* write to pio */
    ret = netwox_remadmtcp_io_write_buf(pbufpool, pbio, pbuf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }

  /* write an empty, followed by error code */
  netwib__buf_reinit(pbuf);
  netwib_er(netwox_remadmtcp_io_write_buf(pbufpool, pbio, pbuf));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
  netwib_er(netwox_remadmtcp_io_write_uint32(pbufpool, pbio, ret));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_io_hash(netwib_bufpool *pbufpool,
                                           netwox_remadmtcp_io *pbio,
                                           netwib_bool calledbyclient,
                                           netwib_constbuf *ppassword,
                                           netwib_buf *phash)
{
  netwib_buf *premoterand, *plocalrand;
  netwox_md_context ctx;
  netwib_err ret;

  /* generate and send local rand */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &plocalrand));
  netwib_er(netwib_buf_append_rand_all(32, plocalrand));
  netwib_er(netwox_remadmtcp_io_write_buf(pbufpool, pbio, plocalrand));

  /* read remote rand */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &premoterand));
  ret = netwox_remadmtcp_io_read_buf(pbio, premoterand);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbufpool, &plocalrand));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &premoterand));
    return(ret);
  }

  /* generate hash */
  netwib_er(netwox_md5_init(&ctx));
  if (calledbyclient) {
    netwib_er(netwox_md_update(&ctx, plocalrand));
    netwib_er(netwox_md_update(&ctx, premoterand));
  } else {
    netwib_er(netwox_md_update(&ctx, premoterand));
    netwib_er(netwox_md_update(&ctx, plocalrand));
  }
  netwib_er(netwox_md_update(&ctx, ppassword));
  netwib_er(netwox_md_final(&ctx, phash));

  /* close */
  netwib_er(netwib_bufpool_buf_close(pbufpool, &plocalrand));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &premoterand));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*

Here is a small explanation of the "protocol" :

Client send
Server send

auth
error_code
serverrand
clientrand
hash(clientrand,serverrand,password)
error_code

quit
error_code

cypher
error_code
serverrandrd
serverrandwr
clientrandrd
clientrandwr
error_code
Note: there is a bug in the protocol, but now it's too late to change.
Indeed, in function netwox_remadmtcp_io_hash, the password is used
to generate the hash. So if client password is bad, all data following
the cypher command end can't be decoded. We obtain a timeout on the
server, instead of a clear message indicating bad password.

exec
cmd
error_code
data
exec_return_code
error_code

get
filename
error_code
data
error_code

put
filename
error_code
data
error_code

Notes :
- all messages are TLV encoded
- when cypher has been activated, the session is encrypted to the end
- data is separated in chunks :
{34 bytes of data}
{12 bytes of data}
{0 byte} [indicates end of data]
error_code

*/

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_REMADMTCP_CMD_AUTH = 1,
  NETWOX_REMADMTCP_CMD_QUIT,
  NETWOX_REMADMTCP_CMD_CYPHER,
  NETWOX_REMADMTCP_CMD_EXEC,
  NETWOX_REMADMTCP_CMD_GET,
  NETWOX_REMADMTCP_CMD_PUT
} netwox_remadmtcp_cmd;

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_ser_auth(netwox_remadmtcp_ser *pbser)
{
  netwib_buf *pclienthash, *pexpectedhash;
  netwib_cmp cmp;
  netwib_err ret;

  /* accept this command */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                             NETWIB_ERR_OK));

  /* generate expected hash */
  netwib_er(netwib_bufpool_buf_init(pbser->pbufpool, &pexpectedhash));
  ret = netwox_remadmtcp_io_hash(pbser->pbufpool, &pbser->io, NETWIB_FALSE,
                                 &pbser->ppara->password, pexpectedhash);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pexpectedhash));
    return(ret);
  }

  /* read client hash */
  netwib_er(netwib_bufpool_buf_init(pbser->pbufpool, &pclienthash));
  ret = netwox_remadmtcp_io_read_buf(&pbser->io, pclienthash);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pclienthash));
    netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pexpectedhash));
    return(ret);
  }

  /* compare hashes */
  netwib_er(netwib_buf_cmp(pclienthash, pexpectedhash, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    ret = NETWOX_ERR_AUTHERROR;
  } else {
    pbser->isauthenticated = NETWIB_TRUE;
  }
  netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io, ret));

  /* close */
  netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pexpectedhash));
  netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pclienthash));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_ser_quit(netwox_remadmtcp_ser *pbser)
{

  /* accept this command */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                             NETWIB_ERR_OK));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_ser_cypher(netwox_remadmtcp_ser *pbser)
{
  netwib_buf *phash, *pkey;
  netwib_err ret;

  /* accept this command */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                             NETWIB_ERR_OK));

  /* set read key */
  netwib_er(netwib_bufpool_buf_init(pbser->pbufpool, &phash));
  ret = netwox_remadmtcp_io_hash(pbser->pbufpool, &pbser->io, NETWIB_FALSE,
                                 &pbser->ppara->password, phash);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &phash));
    return(ret);
  }
  netwib_er(netwib_bufpool_buf_init(pbser->pbufpool, &pkey));
  netwib_er(netwox_xor_gene_key(pbser->pbufpool, phash,
                                NETWOX_REMADMTCP_KEY_LEN, pkey));
  netwib_er(netwox_xor_init(pkey, &pbser->io.xorrd));
  netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pkey));
  netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &phash));

  /* set write key */
  netwib_er(netwib_bufpool_buf_init(pbser->pbufpool, &phash));
  ret = netwox_remadmtcp_io_hash(pbser->pbufpool, &pbser->io, NETWIB_FALSE,
                                 &pbser->ppara->password, phash);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &phash));
    return(ret);
  }
  netwib_er(netwib_bufpool_buf_init(pbser->pbufpool, &pkey));
  netwib_er(netwox_xor_gene_key(pbser->pbufpool, phash,
                                NETWOX_REMADMTCP_KEY_LEN, pkey));
  netwib_er(netwox_xor_init(pkey, &pbser->io.xorwr));
  netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pkey));
  netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &phash));

  /* confirm ok */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                             NETWIB_ERR_OK));

  /* start cyphering now */
  pbser->io.cypher = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_ser_exec(netwox_remadmtcp_ser *pbser)
{
  netwib_buf *pcmd;
  netwib_bool exitednormally;
  netwib_uint32 returnedvalue;
  netwib_io *pio;
  netwib_err ret;

  /* run this command */
  if (!pbser->isauthenticated) {
    netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                               NETWOX_ERR_AUTHNEEDED));
    return(NETWOX_ERR_AUTHNEEDED);
  }
  if (!pbser->ppara->allowexec) {
    netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                               NETWOX_ERR_NOTALLOWED));
    return(NETWOX_ERR_NOTALLOWED);
  }
  netwib_er(netwib_bufpool_buf_init(pbser->pbufpool, &pcmd));
  ret = netwox_remadmtcp_io_read_buf(&pbser->io, pcmd);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pcmd));
    return(ret);
  }
  ret = netwib_io_init_exec(pcmd, NETWIB_IO_WAYTYPE_READ, NETWIB_TRUE,
                            &exitednormally, &returnedvalue, &pio);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pcmd));
    netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                               ret));
    return(ret);
  }
  netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pcmd));

  /* send ok */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                             NETWIB_ERR_OK));

  /* send data */
  netwib_er(netwox_remadmtcp_io_write_io(pbser->pbufpool, &pbser->io, pio));
  netwib_er(netwib_io_close(&pio));

  /* send end */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                             returnedvalue));
  netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                             NETWIB_ERR_OK));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_ser_get(netwox_remadmtcp_ser *pbser)
{
  netwib_buf *pfilename, *precvfilename;
  netwib_io *pio;
  netwib_err ret;

  /* open filename */
  if (!pbser->isauthenticated) {
    netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                               NETWOX_ERR_AUTHNEEDED));
    return(NETWOX_ERR_AUTHNEEDED);
  }
  if (!pbser->ppara->allowget) {
    netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                               NETWOX_ERR_NOTALLOWED));
    return(NETWOX_ERR_NOTALLOWED);
  }
  netwib_er(netwib_bufpool_buf_init(pbser->pbufpool, &precvfilename));
  ret = netwox_remadmtcp_io_read_buf(&pbser->io, precvfilename);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &precvfilename));
    return(ret);
  }
  /* check file */
  netwib_er(netwib_bufpool_buf_init(pbser->pbufpool, &pfilename));
  ret = netwib_path_init_jail(&pbser->ppara->rootdir, precvfilename,
                              pfilename);
  netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &precvfilename));
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pfilename));
    netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                               ret));
    return(ret);
  }
  /* prepare for read */
  ret = netwib_io_init_file_read(pfilename, &pio);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pfilename));
    netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                               ret));
    return(ret);
  }
  netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pfilename));

  /* send ok */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                             NETWIB_ERR_OK));

  /* send data */
  netwib_er(netwox_remadmtcp_io_write_io(pbser->pbufpool, &pbser->io, pio));
  netwib_er(netwib_io_close(&pio));

  /* send end */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                             NETWIB_ERR_OK));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_ser_put(netwox_remadmtcp_ser *pbser)
{
  netwib_buf *pfilename, *precvfilename;
  netwib_io *pio;
  netwib_err ret;

  /* open filename */
  if (!pbser->isauthenticated) {
    netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                               NETWOX_ERR_AUTHNEEDED));
    return(NETWOX_ERR_AUTHNEEDED);
  }
  if (!pbser->ppara->allowput) {
    netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                               NETWOX_ERR_NOTALLOWED));
    return(NETWOX_ERR_NOTALLOWED);
  }
  netwib_er(netwib_bufpool_buf_init(pbser->pbufpool, &precvfilename));
  ret = netwox_remadmtcp_io_read_buf(&pbser->io, precvfilename);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &precvfilename));
    return(ret);
  }
  /* check file */
  netwib_er(netwib_bufpool_buf_init(pbser->pbufpool, &pfilename));
  ret = netwib_path_init_jail(&pbser->ppara->rootdir, precvfilename,
                              pfilename);
  netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &precvfilename));
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pfilename));
    netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                               ret));
    return(ret);
  }
  /* prepare for write */
  ret = netwib_io_init_file_write(pfilename, &pio);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pfilename));
    netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                               ret));
    return(ret);
  }
  netwib_er(netwib_bufpool_buf_close(pbser->pbufpool, &pfilename));

  /* send ok */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                             NETWIB_ERR_OK));

  /* get data */
  netwib_er(netwox_remadmtcp_io_read_io(pbser->pbufpool, &pbser->io, pio));
  netwib_er(netwib_io_close(&pio));

  /* send end */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbser->pbufpool, &pbser->io,
                                             NETWIB_ERR_OK));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_remadmtcp_ser_loop2(netwox_remadmtcp_ser *pbser)
{
  netwox_remadmtcp_cmd cmd;

  while (NETWIB_TRUE) {
    /* read command */
    netwib_er(netwox_remadmtcp_io_read_uint32(&pbser->io,
                                              (netwib_uint32*)&cmd));
    /* do it */
    switch(cmd) {
    case NETWOX_REMADMTCP_CMD_AUTH :
      netwib_er(netwox_remadmtcp_ser_auth(pbser));
      break;
    case NETWOX_REMADMTCP_CMD_QUIT :
      netwib_er(netwox_remadmtcp_ser_quit(pbser));
      return(NETWIB_ERR_OK);
      break;
    case NETWOX_REMADMTCP_CMD_CYPHER :
      netwib_er(netwox_remadmtcp_ser_cypher(pbser));
      break;
    case NETWOX_REMADMTCP_CMD_EXEC :
      netwib_er(netwox_remadmtcp_ser_exec(pbser));
      break;
    case NETWOX_REMADMTCP_CMD_GET :
      netwib_er(netwox_remadmtcp_ser_get(pbser));
      break;
    case NETWOX_REMADMTCP_CMD_PUT :
      netwib_er(netwox_remadmtcp_ser_put(pbser));
      break;
    default :
      return(NETWOX_ERR_PROTOCOL);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_remadmtcp_ser_loop(netwib_io *pio,
                                     netwib_ptr pinfos)
{
  netwox_remadmtcp_ser_para *pbserpara = (netwox_remadmtcp_ser_para *)pinfos;
  netwox_remadmtcp_ser bser;
  netwib_ips *pallowedclients;
  netwib_ip remoteip;
  netwib_bool allowed;
  netwib_err ret;

  /* check client IP */
  netwib_er(netwib_sock_ctl_get_remote(pio, &remoteip, NULL));
  /*netwib_er(netwib_fmt_display("Client %{ip} (%{buf})\n", &remoteip,
    &pbserpara->allowedclients));*/
  netwib_er(netwib_ips_initdefault(&pallowedclients));
  netwib_er(netwib_ips_add_buf(pallowedclients, &pbserpara->allowedclients));
  allowed = NETWIB_FALSE;
  netwib_er(netwib_ips_contains_ip(pallowedclients, &remoteip, &allowed));
  netwib_er(netwib_ips_close(&pallowedclients));
  if (!allowed) {
    netwib_er(netwox_err_display(NETWOX_ERR_NOTALLOWED,
                                 NETWIB_ERR_ENCODETYPE_NUMTEXT));
    return(NETWIB_ERR_OK);
  }

  /* initialize bser */
  bser.isauthenticated = NETWIB_FALSE;
  netwib_er(netwib_bufpool_initdefault(&bser.pbufpool));
  bser.io.pio = pio;
  bser.io.cypher = NETWIB_FALSE;
  bser.io.timeoutsec = pbserpara->timeoutsec;
  netwib_er(netwib_buf_init_mallocdefault(&bser.io.bufrd));
  bser.io.bufrd.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  bser.ppara = pbserpara;

  ret = netwox_remadmtcp_ser_loop2(&bser);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_err_display(ret, NETWIB_ERR_ENCODETYPE_NUMTEXT));
  }

  /* close bser */
  netwib_er(netwib_bufpool_close(&bser.pbufpool));
  if (bser.io.cypher) {
    netwib_er(netwox_xor_close(&bser.io.xorrd));
    netwib_er(netwox_xor_close(&bser.io.xorwr));
  }
  netwib_er(netwib_buf_close(&bser.io.bufrd));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_remadmtcp_cli_init(netwib_io *pio,
                                     netwib_uint32 timeoutsec,
                                     netwox_remadmtcp_cli *pbcli)
{

  netwib_er(netwib_bufpool_initdefault(&pbcli->pbufpool));
  pbcli->io.pio = pio;
  pbcli->io.cypher = NETWIB_FALSE;
  pbcli->io.timeoutsec = timeoutsec;
  netwib_er(netwib_buf_init_mallocdefault(&pbcli->io.bufrd));
  pbcli->io.bufrd.flags |= NETWIB_BUF_FLAGS_CANSLIDE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_remadmtcp_cli_close(netwox_remadmtcp_cli *pbcli)
{

  netwib_er(netwib_bufpool_close(&pbcli->pbufpool));
  if (pbcli->io.cypher) {
    netwib_er(netwox_xor_close(&pbcli->io.xorrd));
    netwib_er(netwox_xor_close(&pbcli->io.xorwr));
  }
  netwib_er(netwib_buf_close(&pbcli->io.bufrd));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_remadmtcp_cli_auth(netwox_remadmtcp_cli *pbcli,
                                     netwib_constbuf *ppassword)
{
  netwib_buf *phash, password;
  netwib_err ret;

  /* command */
  password = *ppassword;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwox_remadmtcp_io_write_uint32(pbcli->pbufpool, &pbcli->io,
                                             NETWOX_REMADMTCP_CMD_AUTH));

  /* check error */
  netwib_er(netwox_remadmtcp_io_read_uint32(&pbcli->io, (netwib_uint32*)&ret));
  netwib_er(ret);

  /* generate and send hash */
  netwib_er(netwib_bufpool_buf_init(pbcli->pbufpool, &phash));
  netwib_er(netwox_remadmtcp_io_hash(pbcli->pbufpool, &pbcli->io, NETWIB_TRUE,
                                     &password, phash));
  netwib_er(netwox_remadmtcp_io_write_buf(pbcli->pbufpool, &pbcli->io, phash));
  netwib_er(netwib_bufpool_buf_close(pbcli->pbufpool, &phash));

  /* check error */
  netwib_er(netwox_remadmtcp_io_read_uint32(&pbcli->io, (netwib_uint32*)&ret));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_remadmtcp_cli_quit(netwox_remadmtcp_cli *pbcli)
{
  netwib_err ret;

  /* command */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbcli->pbufpool, &pbcli->io,
                                             NETWOX_REMADMTCP_CMD_QUIT));

  /* check error */
  netwib_er(netwox_remadmtcp_io_read_uint32(&pbcli->io, (netwib_uint32*)&ret));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_remadmtcp_cli_cypher(netwox_remadmtcp_cli *pbcli,
                                       netwib_constbuf *ppassword)
{
  netwib_buf *phash, *pkey, password;
  netwib_err ret;

  password = *ppassword;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;

  /* command */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbcli->pbufpool, &pbcli->io,
                                             NETWOX_REMADMTCP_CMD_CYPHER));

  /* check error */
  netwib_er(netwox_remadmtcp_io_read_uint32(&pbcli->io, (netwib_uint32*)&ret));
  netwib_er(ret);

  /* set write key */
  netwib_er(netwib_bufpool_buf_init(pbcli->pbufpool, &phash));
  ret = netwox_remadmtcp_io_hash(pbcli->pbufpool, &pbcli->io, NETWIB_TRUE,
                                 &password, phash);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbcli->pbufpool, &phash));
    return(ret);
  }
  netwib_er(netwib_bufpool_buf_init(pbcli->pbufpool, &pkey));
  netwib_er(netwox_xor_gene_key(pbcli->pbufpool, phash,
                                NETWOX_REMADMTCP_KEY_LEN, pkey));
  netwib_er(netwox_xor_init(pkey, &pbcli->io.xorwr));
  netwib_er(netwib_bufpool_buf_close(pbcli->pbufpool, &pkey));
  netwib_er(netwib_bufpool_buf_close(pbcli->pbufpool, &phash));

  /* set read key */
  netwib_er(netwib_bufpool_buf_init(pbcli->pbufpool, &phash));
  ret = netwox_remadmtcp_io_hash(pbcli->pbufpool, &pbcli->io, NETWIB_TRUE,
                                 &password, phash);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbcli->pbufpool, &phash));
    return(ret);
  }
  netwib_er(netwib_bufpool_buf_init(pbcli->pbufpool, &pkey));
  netwib_er(netwox_xor_gene_key(pbcli->pbufpool, phash,
                                NETWOX_REMADMTCP_KEY_LEN, pkey));
  netwib_er(netwox_xor_init(pkey, &pbcli->io.xorrd));
  netwib_er(netwib_bufpool_buf_close(pbcli->pbufpool, &pkey));
  netwib_er(netwib_bufpool_buf_close(pbcli->pbufpool, &phash));

  /* check error */
  netwib_er(netwox_remadmtcp_io_read_uint32(&pbcli->io, (netwib_uint32*)&ret));

  /* start cyphering now */
  pbcli->io.cypher = NETWIB_TRUE;

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_remadmtcp_cli_exec(netwox_remadmtcp_cli *pbcli,
                                     netwib_constbuf *pcmd,
                                     netwib_io *plocalio,
                                     netwib_uint32 *perr)
{
  netwib_err ret;

  /* command */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbcli->pbufpool, &pbcli->io,
                                             NETWOX_REMADMTCP_CMD_EXEC));
  netwib_er(netwox_remadmtcp_io_write_buf(pbcli->pbufpool, &pbcli->io, pcmd));

  /* check error */
  netwib_er(netwox_remadmtcp_io_read_uint32(&pbcli->io, (netwib_uint32*)&ret));
  netwib_er(ret);

  /* obtain data */
  netwib_er(netwox_remadmtcp_io_read_io(pbcli->pbufpool, &pbcli->io, plocalio));

  /* obtain exec error */
  netwib_er(netwox_remadmtcp_io_read_uint32(&pbcli->io, perr));

  /* check error */
  netwib_er(netwox_remadmtcp_io_read_uint32(&pbcli->io, (netwib_uint32*)&ret));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_remadmtcp_cli_get(netwox_remadmtcp_cli *pbcli,
                                    netwib_constbuf *premotefile,
                                    netwib_io *plocalio)
{
  netwib_err ret;

  /* command */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbcli->pbufpool, &pbcli->io,
                                             NETWOX_REMADMTCP_CMD_GET));
  netwib_er(netwox_remadmtcp_io_write_buf(pbcli->pbufpool, &pbcli->io,
                                          premotefile));

  /* check error */
  netwib_er(netwox_remadmtcp_io_read_uint32(&pbcli->io, (netwib_uint32*)&ret));
  netwib_er(ret);

  /* obtain data */
  netwib_er(netwox_remadmtcp_io_read_io(pbcli->pbufpool, &pbcli->io, plocalio));

  /* check error */
  netwib_er(netwox_remadmtcp_io_read_uint32(&pbcli->io, (netwib_uint32*)&ret));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_remadmtcp_cli_put(netwox_remadmtcp_cli *pbcli,
                                    netwib_io *plocalio,
                                    netwib_constbuf *premotefile)
{
  netwib_err ret;

  /* command */
  netwib_er(netwox_remadmtcp_io_write_uint32(pbcli->pbufpool, &pbcli->io,
                                             NETWOX_REMADMTCP_CMD_PUT));
  netwib_er(netwox_remadmtcp_io_write_buf(pbcli->pbufpool, &pbcli->io,
                                          premotefile));

  /* check error */
  netwib_er(netwox_remadmtcp_io_read_uint32(&pbcli->io, (netwib_uint32*)&ret));
  netwib_er(ret);

  /* send data */
  netwib_er(netwox_remadmtcp_io_write_io(pbcli->pbufpool, &pbcli->io,
                                         plocalio));

  /* check error */
  netwib_er(netwox_remadmtcp_io_read_uint32(&pbcli->io, (netwib_uint32*)&ret));

  return(ret);
}

