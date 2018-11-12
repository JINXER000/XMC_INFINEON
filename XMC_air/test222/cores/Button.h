#ifndef __BOARD_BUTTONS_H
#define __BOARD_BUTTONS_H

#include <stdint.h>

/**
  \fn          int32_t Buttons_Initialize (void)
  \brief       Initialize buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          int32_t Buttons_Uninitialize (void)
  \brief       De-initialize buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          uint32_t Buttons_GetState (void)
  \brief       Get buttons state
  \returns     Buttons state
*/
/**
  \fn          uint32_t Buttons_GetCount (void)
  \brief       Get number of available buttons
  \return      Number of available buttons
*/
#ifdef __cplusplus
extern "C" {
#endif

extern int32_t  Buttons_Initialize   (void);
extern int32_t  Buttons_Uninitialize (void);
extern uint32_t Buttons_GetState     (void);
extern uint32_t Buttons_GetCount     (void);
#ifdef __cplusplus
}
#endif	
#endif /* __BOARD_BUTTONS_H */