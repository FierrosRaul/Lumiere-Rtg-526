#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "lumiere.h"


#ifdef __cplusplus
 extern "C"
#endif
void MCU_init(void); /* Device initialization function declaration */

void main(void) {
  MCU_init(); /*  call Device Initialization */

  /* include your code here */


	Main();
  /* please make sure that you never leave main */
}
