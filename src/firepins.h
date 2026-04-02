/* 
 * File:   firepins.h
 * Author: adt1
 *
 * Created on February 27, 2026, 9:49 PM
 */

#ifndef FIREPINS_H
#define	FIREPINS_H

#ifdef	__cplusplus
extern "C" {
#endif
_Bool pinSet_2(_Bool [],_Bool []);
_Bool pinSet_1(_Bool []);
_Bool cellSelect(_Bool [], _Bool [], _Bool []);
void pinInit();
void fire(uint8_t * , uint32_t);


#ifdef	__cplusplus
}
#endif

#endif	/* FIREPINS_H */

