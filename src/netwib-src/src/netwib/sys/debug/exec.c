/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_debug_exec_init(netwib_conststring command,
                                  netwib_uint32 *ppid,
                                  int *pfd)
{
  int pipefd[2];
  int reti, pid;

  reti = pipe(pipefd);
  if (reti == -1)
    return(NETWIB_ERR_FUPIPE);

  pid = fork();
  if (pid == -1) {
    close(pipefd[0]);
    close(pipefd[1]);
    return(NETWIB_ERR_FUFORK);
  }
  if (pid == 0) {
    /* child */
    reti = close(pipefd[0]);
    if (reti == -1)
      return(NETWIB_ERR_FUCLOSE);
    reti = close(STDOUT_FILENO);
    if (reti == -1)
      return(NETWIB_ERR_FUCLOSE);
    reti = close(STDERR_FILENO);
    if (reti == -1)
      return(NETWIB_ERR_FUCLOSE);
    reti = dup2(pipefd[1], STDOUT_FILENO);
    if (reti == -1)
      return(NETWIB_ERR_FUDUP2);
    reti = dup2(pipefd[1], STDERR_FILENO);
    if (reti == -1)
      return(NETWIB_ERR_FUDUP2);
    /* we cast NULL to void* because under OpenBSD, NULL is defined as
       0 (instead of (void)*0), and gcc warns saying there is no sentinel
       at the end of execle */
    reti = execle("/bin/sh", "/bin/sh", "-c", command, NULL, (void*)NULL);
    if (reti == -1)
      return(NETWIB_ERR_FUEXECLE);
  }
  /* father */
  reti = close(pipefd[1]);
  if (reti == -1)
    return(NETWIB_ERR_FUCLOSE);
  if (pfd != NULL)
    *pfd = pipefd[0];
  if (ppid != NULL)
    *ppid = pid;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_exec_close(netwib_uint32 pid,
                                   int fd)
{
  int reti, status;

  reti = close(fd);
  if (reti == -1)
    return(NETWIB_ERR_FUCLOSE);
  /* eventually kill the child */
  /* ignore */ kill(pid, SIGTERM);
  /* ignore */ waitpid(pid, &status, 0);
  errno = 0;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_exec_line(int fd,
                                  netwib_string *pstringout)
{
  netwib_char stringout[512];
  netwib_uint32 stringoutsize;
  netwib_char c;
  int reti;

  netwib_er(netwib_debug_string_init(pstringout));

  stringoutsize = 0;
  while (NETWIB_TRUE) {
    reti = read(fd, &c, 1);
    if (reti == -1) {
      if (stringoutsize != 0) {
        /* output the received size */
        stringout[stringoutsize] = '\0';
        netwib_er(netwib_debug_string_append_string(stringout, pstringout));
        return(NETWIB_ERR_OK);
      }
      if (errno == EBADF) {
        /* reached end */
        errno = 0;
        netwib_er(netwib_debug_string_close(pstringout));
        return(NETWIB_ERR_DATAEND);
      }
      /* fatal error */
      netwib_er(netwib_debug_string_close(pstringout));
      return(NETWIB_ERR_FUREAD);
    }
    if (reti == 0) {
      /* no more data */
      if (stringoutsize != 0) {
        /* output the received size */
        stringout[stringoutsize] = '\0';
        netwib_er(netwib_debug_string_append_string(stringout, pstringout));
        return(NETWIB_ERR_OK);
      }
      /* reached end */
      netwib_er(netwib_debug_string_close(pstringout));
      return(NETWIB_ERR_DATAEND);
    }
    /* terminate at end of line */
    if (c == '\0' || c == '\n') {
      stringout[stringoutsize] = '\0';
      netwib_er(netwib_debug_string_append_string(stringout, pstringout));
      return(NETWIB_ERR_OK);
    }
    /* ignore this character */
    if (c == '\r') {
      continue;
    }
    /* set buffer */
    stringout[stringoutsize++] = c;
    /* append */
    if (stringoutsize > sizeof(stringout) - 2) {
      stringout[stringoutsize] = '\0';
      netwib_er(netwib_debug_string_append_string(stringout, pstringout));
      stringoutsize = 0;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_exec_line2(int fd,
                                   netwib_uint32 stringoutmaxsize,
                                   netwib_string stringout)
{
  netwib_uint32 stringoutsize;
  netwib_char c;
  int reti;

  if (stringoutmaxsize == 0) {
    return(NETWIB_ERR_PATOOLOW);
  }

  stringoutsize = 0;
  while (NETWIB_TRUE) {
    reti = read(fd, &c, 1);
    if (reti == -1) {
      if (stringoutsize != 0) {
        /* output the received size */
        stringout[stringoutsize] = '\0';
        return(NETWIB_ERR_OK);
      }
      if (errno == EBADF) {
        /* reached end */
        errno = 0;
        return(NETWIB_ERR_DATAEND);
      }
      /* fatal error */
      return(NETWIB_ERR_FUREAD);
    }
    if (reti == 0) {
      /* no more data */
      if (stringoutsize != 0) {
        /* output the received size */
        stringout[stringoutsize] = '\0';
        return(NETWIB_ERR_OK);
      }
      /* reached end */
      return(NETWIB_ERR_DATAEND);
    }
    /* terminate at end of line */
    if (c == '\0' || c == '\n') {
      stringout[stringoutsize] = '\0';
      return(NETWIB_ERR_OK);
    }
    /* ignore this character */
    if (c == '\r') {
      continue;
    }
    /* set buffer */
    stringout[stringoutsize++] = c;
    /* truncate */
    if (stringoutsize >= stringoutmaxsize) {
      /* buffer was not sufficient, we've set data but it should not
         be used */
      return(NETWIB_ERR_DATANOSPACE);
    }
  }

  return(NETWIB_ERR_OK);
}
