
/*-------------------------------------------------------------*/
/***************************************************************
 * Functions herein permit to change global configuration of   *
 * netwib.                                                     *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* See below for the meaning of those values */
typedef enum {
  NETWIB_GLOBAL_CTLTYPE_ERR_PURGE = 1,
  NETWIB_GLOBAL_CTLTYPE_CONF_UPDATE,
  NETWIB_GLOBAL_CTLTYPE_DEBUG_CTRLC_PRESSED
} netwib_global_ctltype;
netwib_err netwib_global_ctl_set(netwib_global_ctltype type,
                                 netwib_ptr p,
                                 netwib_uint32 ui);
netwib_err netwib_global_ctl_get(netwib_global_ctltype type,
                                 netwib_ptr p,
                                 netwib_uint32 *pui);

/*-------------------------------------------------------------*/
/* purge last error */
/* netwib_err f(void); */
#define netwib_global_ctl_set_err_purge() netwib_global_ctl_set(NETWIB_GLOBAL_CTLTYPE_ERR_PURGE,NULL,0)

/*-------------------------------------------------------------*/
/* update configuration. Care must be taken to ensure nobody
   is currently looping through current configuration
   (lock/mutex). */
/* netwib_err f(void); */
#define netwib_global_ctl_set_conf_update() netwib_global_ctl_set(NETWIB_GLOBAL_CTLTYPE_CONF_UPDATE,NULL,0)

/*-------------------------------------------------------------*/
/* if Control-C was pressed (used only in Debug mode under Linux,
   in order to detect memory leaks) */
/* netwib_err f(netwib_bool *pbool); */
#define netwib_global_ctl_get_debug_ctrlc_pressed(pbool) netwib_global_ctl_get(NETWIB_GLOBAL_CTLTYPE_DEBUG_CTRLC_PRESSED,NULL,(netwib_uint32 *)pbool)
#define netwib__debug_ctrlc_pressed_break() { netwib_err netwib__debug_ctrlc_pressed_break_ret; netwib_bool netwib__debug_ctrlc_pressed_break_pressed; netwib__debug_ctrlc_pressed_break_ret = netwib_global_ctl_get_debug_ctrlc_pressed(&netwib__debug_ctrlc_pressed_break_pressed); if (netwib__debug_ctrlc_pressed_break_ret == NETWIB_ERR_OK && netwib__debug_ctrlc_pressed_break_pressed) break; }
