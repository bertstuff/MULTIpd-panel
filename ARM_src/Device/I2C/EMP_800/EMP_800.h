#ifndef __EMP_800_H__
#define __EMP_800_H__

#define EMP800_MAX_SPEED_HZ 400000

extern uint8_t EMP_800_event;

/*
 * @brief init the emp 800.00 muntproever
 *
 */
void EMP_800_init();

/*
 * @brief enable coin acceptance
 */
void EMP_800_enable(void);

/*
 * @brief disable coin acceptance
 */
void EMP_800_disable(void);

/*
 * @brief get total value
 */
int32_t EMP_800_get_value(void);

/*
 * @brief get last added value
 */
int32_t EMP_800_get_value_added(void);

/*
 * @brief clear value
 */
void EMP_800_reset_value(void);


/*
 * @brief don't accept coins
 */
void EMP_800_set_blocking(bool enable);

#endif //__EMP_800_H__
