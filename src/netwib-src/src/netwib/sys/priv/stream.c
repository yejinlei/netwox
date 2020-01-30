/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_stream_read(NETWIBFILE *pfile,
                                   netwib_buf *pbuf)
{
  netwib_data data;
  netwib_uint32 maxmsgsize;
  int readsize;

  netwib_er(netwib_buf_wishspace(pbuf, NETWIB_PRIV_MINMSGSIZE_READ,
                                 &data, &maxmsgsize));
  if (maxmsgsize == 0) {
    return(NETWIB_ERR_DATANOSPACE);
  }

  /* read */
  readsize = fread(data, 1, maxmsgsize, (FILE*)pfile);
  /* an error occurred */
  if (readsize == 0) {
    if (feof((FILE*)pfile)) {
      /* reached end */
      return(NETWIB_ERR_DATAEND);
    } else {
      /* fatal error */
      return(NETWIB_ERR_FUFREAD);
    }
  }

  /* store read data */
  pbuf->endoffset += readsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_stream_write(NETWIBFILE *pfile,
                                    netwib_constbuf *pbuf)
{
  netwib_data data;
  netwib_uint32 datasize;
  int reti;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  reti = fwrite(data, datasize, 1, (FILE*)pfile);
  if ((netwib_uint32)reti != 1) {
    /* should we use a loop and try to write missing ? */
    return(NETWIB_ERR_FUFWRITE);
  }

  reti = fflush((FILE*)pfile);
  if (reti) return(NETWIB_ERR_FUFFLUSH);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_stream_wait(NETWIBFILE *pfile,
                                   netwib_io_waytype way,
                                   netwib_consttime *pabstime,
                                   netwib_bool *pevent)
{

#if defined NETWIBDEF_SYSNAME_Unix
  {
    int fd;

    fd = fileno((FILE*)pfile);
    if (fd == -1) return(NETWIB_ERR_FUFILENO);

    netwib_er(netwib_priv_fd_wait(fd, way, pabstime, pevent));
  }
#elif defined NETWIBDEF_SYSNAME_Windows
  if (pevent != NULL) {
    netwib_bool elapsed;
    /* suppose it's a filehandle for a disk file, for which there is
       only data, or endoffile. So there is no need to wait. */
    *pevent = NETWIB_TRUE;
    if (pabstime != NETWIB_TIME_ZERO && pabstime != NETWIB_TIME_INFINITE) {
      netwib_er(netwib_time_iselapsed(pabstime, &elapsed));
      if (elapsed) {
        *pevent = NETWIB_FALSE;
      }
    }
  }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  pfile = pfile; /* for compiler warning */
  way = way; /* for compiler warning */
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
netwib_err netwib_priv_stream_flush(NETWIBFILE *pfile)
{
  int reti;

  reti = fflush((FILE*)pfile);
  if (reti) return(NETWIB_ERR_FUFFLUSH);

  return(NETWIB_ERR_OK);
}
