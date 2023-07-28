
/*
 * @file           : kfifo.h
 * @autor          : liyou
 * @date           : 2022/01/16
 */

#ifndef __KFIFO_H
#define __KFIFO_H

#include "kservice.h"

#define min(x, y) ((x) < (y) ? (x) : (y))

struct kfifo 
{
	unsigned int    in;
	unsigned int    out;
	unsigned int	mask;
	unsigned char *data;
};

unsigned int __kfifo_init(struct kfifo *fifo, void *buffer,
		unsigned int size, unsigned int esize);

unsigned int kfifo_in(struct kfifo *fifo,
		const unsigned char *buf, unsigned int len);

unsigned int kfifo_out(struct kfifo *fifo,
		void *buf, unsigned int len);

#define kfifo_init(fifo, buffer, size) \
	__kfifo_init(fifo, buffer, size, 1)

#endif
