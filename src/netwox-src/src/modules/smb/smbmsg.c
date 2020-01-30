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
#include "../../netwox.h"

/*-------------------------------------------------------------*/
netwib_err netwox_smbmsg_init(netwox_smbmsg *psmbmsg)
{
  netwib_uint32 i;

  for (i = 0; i < NETWOX_SMBMSG_ANDX_MAX; i++) {
    netwib_er(netwib_buf_init_ext_storage(&psmbmsg->cmds[i].para));
    netwib_er(netwib_buf_init_ext_storage(&psmbmsg->cmds[i].data));
    psmbmsg->cmds[i].dataisaligned = NETWIB_FALSE; /* to have cmd aligned */
  }
  psmbmsg->numcmds = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbmsg_setdefault(netwox_smbmsg *psmbmsg)
{
  netwib_uint32 i;

  psmbmsg->hdr.error = 0;
  psmbmsg->hdr.flags = NETWOX_SMBMSG_FLAGS_CANONICALPATHNAMES;
  psmbmsg->hdr.flags2 = 0;
  psmbmsg->hdr.treeid = 0;
  psmbmsg->hdr.processid = 0;
  psmbmsg->hdr.userid = 0;
  psmbmsg->hdr.multiplexid = 0;
  netwib_c_memset(psmbmsg->hdr.signature, 0, sizeof(psmbmsg->hdr.signature));

  for (i = 0; i < psmbmsg->numcmds; i++) {
    netwib__buf_reinit(&psmbmsg->cmds[i].para);
    netwib__buf_reinit(&psmbmsg->cmds[i].data);
    psmbmsg->cmds[i].dataisaligned = NETWIB_FALSE; /* to have cmd aligned */
  }
  psmbmsg->numcmds = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbmsg_close(netwox_smbmsg *psmbmsg)
{
  netwib_uint32 i;

  for (i = 0; i < NETWOX_SMBMSG_ANDX_MAX; i++) {
    netwib_er(netwib_buf_close(&psmbmsg->cmds[i].para));
    netwib_er(netwib_buf_close(&psmbmsg->cmds[i].data));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_cmd_isandx(netwox_smbmsg_cmd cmd,
                                           netwib_bool *pisandx)
{

  switch(cmd) {
  case NETWOX_SMBMSG_CMD_LOCKINGANDX :
  case NETWOX_SMBMSG_CMD_OPENANDX :
  case NETWOX_SMBMSG_CMD_READANDX :
  case NETWOX_SMBMSG_CMD_WRITEANDX :
  case NETWOX_SMBMSG_CMD_SESSIONSETUPANDX :
  case NETWOX_SMBMSG_CMD_LOGOFFANDX :
  case NETWOX_SMBMSG_CMD_TREECONNECTANDX :
  case NETWOX_SMBMSG_CMD_NTCREATEANDX :
    *pisandx = NETWIB_TRUE;
    break;
  default :
    *pisandx = NETWIB_FALSE;
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_smbmsg(netwox_constsmbmsg *psmbmsg,
                                    netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 i, j, wordcount, bytecount, andxoffset, padsize;
  netwib_bool isandx, paraneedpadding;

  if (psmbmsg->numcmds == 0) {
    return(NETWIB_ERR_PATOOLOW);
  }
  if (psmbmsg->numcmds > NETWOX_SMBMSG_ANDX_MAX) {
    return(NETWIB_ERR_PATOOHIGH);
  }

  /* header */
  netwib_er(netwib_buf_wantspace(ppkt, NETWOX_SMBMSGHDR_LEN, &data));
  netwib__data_append_uint8_le(data, 0xFF);
  netwib__data_append_uint8_le(data, 'S');
  netwib__data_append_uint8_le(data, 'M');
  netwib__data_append_uint8_le(data, 'B');
  netwib__data_append_uint8_le(data, psmbmsg->cmds[0].cmd);
  netwib__data_append_uint32(data, psmbmsg->hdr.error); /* bigendian */
  netwib__data_append_uint8_le(data, psmbmsg->hdr.flags);
  netwib__data_append_uint16_le(data, psmbmsg->hdr.flags2);
  /* extra field contains high_pid(8).signature(8*8).reserved(16) */
  netwib__data_append_uint16_le(data, psmbmsg->hdr.processid>>16);
  netwib_c_memcpy(data, psmbmsg->hdr.signature, 8);
  data += 8;
  netwib__data_append_uint16_le(data, 0); /* reserved */
  netwib__data_append_uint16_le(data, psmbmsg->hdr.treeid);
  netwib__data_append_uint16_le(data, psmbmsg->hdr.processid&0xFFFF);
  netwib__data_append_uint16_le(data, psmbmsg->hdr.userid);
  netwib__data_append_uint16_le(data, psmbmsg->hdr.multiplexid);
  ppkt->endoffset += NETWOX_SMBMSGHDR_LEN;

  /* bodies */
  andxoffset = NETWOX_SMBMSGHDR_LEN;
  padsize = 0;
  for (i = 0; i < psmbmsg->numcmds; i++) {
    if (i == 0) {
      /* only UNaligned data (aligned command, see smbmsg.h) is supported */
      if (psmbmsg->cmds[i].dataisaligned) {
        return(NETWOX_ERR_BADVALUE);
      }
    }
    /* check for AndX commands which have a special header */
    netwib_er(netwox_smbmsg_cmd_isandx(psmbmsg->cmds[i].cmd, &isandx));
    if (!isandx) {
      /* a non AndX must be the last command */
      if (i+1 != psmbmsg->numcmds) {
        return(NETWOX_ERR_BADVALUE);
      }
    }
    /* compute wordcount */
    wordcount = netwib__buf_ref_data_size(&psmbmsg->cmds[i].para);
    if (isandx) wordcount += NETWOX_SMBMSGANDX_LEN;
    paraneedpadding = NETWIB_FALSE;
    if (wordcount & 1) {
      paraneedpadding = NETWIB_TRUE;
      wordcount++;
    }
    if (wordcount > 0x1FE) { /* 0xFF*2, because will be divided by 2 */
      return(NETWIB_ERR_PATOOHIGH);
    }
    /* compute bytecount */
    bytecount = netwib__buf_ref_data_size(&psmbmsg->cmds[i].data);
    if (bytecount > 0xFFFF) {
      return(NETWIB_ERR_PATOOHIGH);
    }
    /* append padding between commands to align them */
    for (j = 0; j < padsize; j++) {
      netwib_er(netwib_buf_append_byte(0, ppkt));
    }
    /* append wordcount (important: it has to be divided by 2 when set) */
    netwib_er(netwib_buf_append_byte((netwib_byte)(wordcount/2), ppkt));
    /* append AndX special header */
    if (isandx) {
      netwib_er(netwib_buf_wantspace(ppkt, NETWOX_SMBMSGANDX_LEN, &data));
      /* append next command */
      if (i+1 == psmbmsg->numcmds) {
        netwib__data_append_uint8_le(data, NETWOX_SMBMSG_CMD_ENDOFLIST);
      } else {
        netwib__data_append_uint8_le(data, psmbmsg->cmds[i+1].cmd);
      }
      /* append reserved */
      netwib__data_append_uint8_le(data, 0);
      /* append AndX offset */
      if (i+1 == psmbmsg->numcmds) {
        netwib__data_append_uint16_le(data, 0);
      } else {
        andxoffset += 1 + wordcount + 2 + bytecount;
        /* check if padding will be needed */
        if (psmbmsg->cmds[i+1].dataisaligned) {
          /* data is aligned, so command must be unaligned */
          padsize = (andxoffset%2)?0:1;
        } else {
          /* data is unaligned, so command must be aligned */
          padsize = andxoffset % 2;
        }
        andxoffset += padsize;
        if (andxoffset > 0xFFFF) {
          return(NETWIB_ERR_PATOOHIGH);
        }
        netwib__data_append_uint16_le(data, andxoffset);
      }
      ppkt->endoffset += NETWOX_SMBMSGANDX_LEN;
    }
    /* append parameters */
    netwib_er(netwib_buf_append_buf(&psmbmsg->cmds[i].para, ppkt));
    if (paraneedpadding) {
      netwib_er(netwib_buf_append_byte(0, ppkt));
    }
    /* append bytecount */
    netwib_er(netwib_buf_wantspace(ppkt, 2, &data));
    netwib__data_append_uint16_le(data, bytecount);
    ppkt->endoffset += 2;
    /* append data */
    netwib_er(netwib_buf_append_buf(&psmbmsg->cmds[i].data, ppkt));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_smbmsg(netwib_constbuf *ppkt,
                                    netwox_smbmsg *psmbmsg)
{
  netwib_data data, dataori;
  netwib_uint32 datasize, datasizeori, tmp;
  netwib_uint32 wordcount, bytecount, andxoffset=0;
  netwib_bool isandx;
  netwox_smbmsg_cmd nextcmd;

  datasizeori = netwib__buf_ref_data_size(ppkt);
  datasize = datasizeori;
  if (datasize < NETWOX_SMBMSGHDR_LEN) {
    return(NETWIB_ERR_DATAMISSING);
  }
  dataori = netwib__buf_ref_data_ptr(ppkt);
  data = dataori;

  /* decode header */
  if (data[0] != 0xFF || data[1] != 'S' || data[2] != 'M' || data[3] != 'B') {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  data += 4;
  netwib__data_decode_uint8_le(data, psmbmsg->cmds[0].cmd);
  netwib__data_decode_uint32(data, psmbmsg->hdr.error); /* bigendian */
  netwib__data_decode_uint8_le(data, psmbmsg->hdr.flags);
  netwib__data_decode_uint16_le(data, psmbmsg->hdr.flags2);
  netwib__data_decode_uint16_le(data, tmp);
  netwib_c_memcpy(psmbmsg->hdr.signature, data, 8);
  data += 8;
  data += 2; /* reserved */
  netwib__data_decode_uint16_le(data, psmbmsg->hdr.treeid);
  netwib__data_decode_uint16_le(data, psmbmsg->hdr.processid);
  psmbmsg->hdr.processid = (tmp<<16) | psmbmsg->hdr.processid;
  netwib__data_decode_uint16_le(data, psmbmsg->hdr.userid);
  netwib__data_decode_uint16_le(data, psmbmsg->hdr.multiplexid);
  datasize -= NETWOX_SMBMSGHDR_LEN;

  /* decode bodies */
  psmbmsg->numcmds = 0;
  while(NETWIB_TRUE) {
    /* decode wordcount */
    if (datasize < 1) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint8_le(data, wordcount);
    datasize--;
    /* Windows 2000 sets a wordcount of 42 instead of 34 for
       NETWOX_SMBMSG_CMD_NTCREATEANDX replies. We have to workaround
       this bug.
    */
    if (psmbmsg->cmds[psmbmsg->numcmds].cmd == NETWOX_SMBMSG_CMD_NTCREATEANDX
        && wordcount == 42 && (psmbmsg->hdr.flags&NETWOX_SMBMSG_FLAGS_REPLY)) {
      wordcount = 34;
    }
    wordcount *= 2;
    if (wordcount > datasize) {
      return(NETWIB_ERR_DATAMISSING);
    }
    /* decode AndX */
    nextcmd = NETWOX_SMBMSG_CMD_ENDOFLIST;
    if (wordcount) {
      netwib_er(netwox_smbmsg_cmd_isandx(psmbmsg->cmds[psmbmsg->numcmds].cmd,
                                         &isandx));
      if (isandx) {
        if (datasize < 4) {
          return(NETWIB_ERR_DATAMISSING);
        }
        netwib__data_decode_uint8_le(data, nextcmd);
        data++;
        netwib__data_decode_uint16_le(data, andxoffset);
        wordcount -= 4;
        datasize -= 4;
      }
    }
    /* decode parameters */
    netwib_er(netwib_buf_init_ext_arrayfilled(data, wordcount, &psmbmsg->cmds[psmbmsg->numcmds].para));
    data += wordcount;
    datasize -= wordcount;
    /* decode bytecount */
    if (datasize < 2) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint16_le(data, bytecount);
    datasize -= 2;
    if (bytecount > datasize) {
       return(NETWIB_ERR_DATAMISSING);
    }
    /* decode data */
    netwib_er(netwib_buf_init_ext_arrayfilled(data, bytecount, &psmbmsg->cmds[psmbmsg->numcmds].data));
    if ((data - dataori) % 2) {
      psmbmsg->cmds[psmbmsg->numcmds].dataisaligned = NETWIB_FALSE;
    } else {
      psmbmsg->cmds[psmbmsg->numcmds].dataisaligned = NETWIB_TRUE;
    }
    /* indicate one more body set */
    psmbmsg->numcmds++;
    /* detect end */
    if (nextcmd == NETWOX_SMBMSG_CMD_ENDOFLIST) {
      break;
    }
    /* too many commands which we cannot store */
    if (psmbmsg->numcmds == NETWOX_SMBMSG_ANDX_MAX) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    psmbmsg->cmds[psmbmsg->numcmds].cmd = nextcmd;
    /* and position for next */
    if (andxoffset > datasizeori) {
      return(NETWIB_ERR_DATAMISSING);
    }
    data = dataori + andxoffset;
    datasize = datasizeori - andxoffset;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbmsg_show(netwox_constsmbmsg *psmbmsg,
                              netwib_buf *pbuf)
{
  netwib_buf signature, errmsg;
  netwox_smbcmd smbcmd;
  netwib_uint32 i;
  netwib_err ret;

  netwib_er(netwib_show_array_head("SMB", pbuf));

  netwib_er(netwib_show_array_fmt32(pbuf, " error=%{uint32:08X}",
                                    psmbmsg->hdr.error));
  netwib_er(netwib_buf_init_mallocdefault(&errmsg));
  if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_NTERROR) {
    netwib_uint8 level;
    netwib_uint16 code;
    level = netwox_smberr_nt_level(psmbmsg->hdr.error);
    code = netwox_smberr_nt_code(psmbmsg->hdr.error);
    netwib_er(netwox_smberr_nt_show(level, code, &errmsg));
  } else {
    netwib_uint8 cl;
    netwib_uint16 code;
    cl = netwox_smberr_dos_class(psmbmsg->hdr.error);
    code = netwox_smberr_dos_code(psmbmsg->hdr.error);
    netwib_er(netwox_smberr_dos_show(cl, code, &errmsg));
  }
  netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}", &errmsg));
  netwib_er(netwib_buf_close(&errmsg));
  netwib_er(netwib_show_array_fmt32(pbuf, " flags=%{uint8:8b}",
                                    psmbmsg->hdr.flags));
  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> message is a reply"));
  }
  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_BATCHOPLOCK) {
    if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
      netwib_er(netwib_show_array_fmt32(pbuf, "  -> batch oplock granted"));
    } else {
      netwib_er(netwib_show_array_fmt32(pbuf, "  -> batch oplock requested"));
    }
  }
  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_OPLOCK) {
    if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
      netwib_er(netwib_show_array_fmt32(pbuf, "  -> oplock granted"));
    } else {
      netwib_er(netwib_show_array_fmt32(pbuf, "  -> oplock requested"));
    }
  }
  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_CANONICALPATHNAMES) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> pathnames are canonicalized"));
  }
  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_CASELESSPATHNAMES) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> pathnames are caseless"));
  }
  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_CLIENTBUFAVAIL) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> client buffer is ready to receive data"));
  }
  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_SUPPORTOLDLOCKCMDS) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> LOCK_AND_READ and WRITE_AND_UNLOCK commands are supported"));
  }
  netwib_er(netwib_show_array_fmt32(pbuf, " flags2=%{uint16:16b}",
                                    psmbmsg->hdr.flags2));
  if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> strings are in Unicode (UTF-16)"));
  }
  if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_NTERROR) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> error field has NT syntax"));
  }
  if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_READIFEXECUTE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> if file has execute permission, read is allowed"));
  }
  if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_DFSPATHNAMES) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> pathnames use DFS (Distributed File system) syntax"));
  }
  if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_EXTENDEDSECURITYNEGO) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Extended Security Negotiation is supported"));
  }
  if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_USESLONGPATHNAME) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> this message uses long pathnames"));
  }
  if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_SECURITYSIGNATURE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> signature of SMB messages is supported"));
  }
  if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_EXTENDEDATTRIBUTES) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Extended Attributes are supported"));
  }
  if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_ACCEPTSLONGPATHNAME) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> long pathnames are supported"));
  }
  netwib_er(netwib_show_array_fmt32(pbuf, " treeid=%{uint16}",
                                    psmbmsg->hdr.treeid));
  netwib_er(netwib_show_array_fmt32(pbuf, " processid=%{uint32}",
                                    psmbmsg->hdr.processid));
  netwib_er(netwib_show_array_fmt32(pbuf, " userid=%{uint16}",
                                    psmbmsg->hdr.userid));
  netwib_er(netwib_show_array_fmt32(pbuf, " multiplexid=%{uint16}",
                                    psmbmsg->hdr.multiplexid));
  netwib_er(netwib_buf_init_ext_arrayfilled((netwib_ptr)(netwib_intptr)psmbmsg->hdr.signature,
                                            sizeof(psmbmsg->hdr.signature),
                                            &signature));
  netwib_er(netwib_show_array_data(" signature", &signature,
                                   NETWIB_ENCODETYPE_HEXA0, ' ', pbuf));

  netwib_er(netwib_show_array_fmt32(pbuf, " numcmds=%{uint32}",
                                    psmbmsg->numcmds));

  for (i = 0; i < psmbmsg->numcmds; i++) {
    netwib_er(netwib_show_array_fmt32(pbuf, " --begin of command %{uint32}",
                                      i));
    netwib_er(netwib_show_array_fmt32(pbuf, " cmd[%{uint32}]=%{uint32} (%{uint32:#02X}) (dataisaligned=%{bool})", i, psmbmsg->cmds[i].cmd, psmbmsg->cmds[i].cmd, psmbmsg->cmds[i].dataisaligned));
    netwib_er(netwox_smbcmd_init(&smbcmd));
    ret = netwox_smbmsg_decode_smbcmd(psmbmsg, i, &smbcmd);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_show_array_data(" para", &psmbmsg->cmds[i].para,
                                       NETWIB_ENCODETYPE_HEXA0, ' ', pbuf));
      netwib_er(netwib_show_array_data(" data", &psmbmsg->cmds[i].data,
                                       NETWIB_ENCODETYPE_HEXA0, ' ', pbuf));
    } else {
      netwib_er(netwox_smbcmd_show(&smbcmd, pbuf));
    }
    netwib_er(netwox_smbcmd_close(&smbcmd));
    netwib_er(netwib_show_array_fmt32(pbuf, " --end of command %{uint32}", i));
  }

  netwib_er(netwib_show_array_tail(pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbmsg_display(netwib_bufpool *pbufpool,
                                 netwox_constsmbmsg *psmbmsg)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  ret = netwox_smbmsg_show(psmbmsg, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  } else {
    netwib_er(netwib_fmt_display("This SMB message could not be decoded\n"));
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbmsg_error_display(netwib_bufpool *pbufpool,
                                       netwox_constsmbmsg *psmbmsg)
{
  netwib_buf *pbuf;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  netwib_er(netwib_buf_append_string("Error: ", pbuf));
  if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_NTERROR) {
    netwib_uint8 level;
    netwib_uint16 code;
    level = netwox_smberr_nt_level(psmbmsg->hdr.error);
    code = netwox_smberr_nt_code(psmbmsg->hdr.error);
    netwib_er(netwox_smberr_nt_show(level, code, pbuf));
  } else {
    netwib_uint8 cl;
    netwib_uint16 code;
    cl = netwox_smberr_dos_class(psmbmsg->hdr.error);
    code = netwox_smberr_dos_code(psmbmsg->hdr.error);
    netwib_er(netwox_smberr_dos_show(cl, code, pbuf));
  }
  netwib_er(netwib_buf_append_string("\n", pbuf));

  netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}
