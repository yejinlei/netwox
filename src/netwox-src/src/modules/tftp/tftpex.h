
/*-------------------------------------------------------------*/
netwib_err netwox_tftp_cli2ser(netwox_sockinfo *psockinfo,
                               netwib_io **ppio);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwib_uint32 maxwaitms;
  netwib_uint32 retry;
  netwib_io *pio;
} netwox_tftpinfos;

/*-------------------------------------------------------------*/
netwib_err netwox_tftp_send(netwox_tftpinfos *ptftpinfos,
                            netwox_consttftppkt *ptftppktsend);
netwib_err netwox_tftp_recv(netwox_tftpinfos *ptftpinfos,
                            netwox_tftppkt *ptftppktrecv);

/*-------------------------------------------------------------*/
netwib_err netwox_tftp_write(netwox_tftpinfos *ptftpinfos,
                             netwib_io *plocalio,
                             netwib_bool server);
netwib_err netwox_tftp_read(netwox_tftpinfos *ptftpinfos,
                            netwib_io *plocalio,
                            netwib_bool server);
