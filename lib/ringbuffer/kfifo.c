
/*
 * @file           : kfifo.c
 * @autor          : liyou
 * @date           : 2022/01/16
 */

#include "kfifo.h"

unsigned int __kfifo_init(struct kfifo *fifo,
	void *buffer, unsigned int size, unsigned int esize)
{
	size /= esize; 

	fifo->in = 0;
	fifo->out = 0;
	fifo->data = buffer;
	fifo->mask = size - 1;

	return 0;
}

static void kfifo_copy_in(struct kfifo *fifo, 
	const unsigned char *src, unsigned int len, unsigned int off)
{
	unsigned int size = fifo->mask + 1;
	unsigned int l;

	off &= fifo->mask;

	l = min(len, size - off);		
		
	ly_memcpy(fifo->data + off, src, l);

	ly_memcpy(fifo->data, src + l, len - l);
}

unsigned int kfifo_in(struct kfifo *fifo,
		const unsigned char* buf, unsigned int len)
{
	unsigned int l;

	l = (fifo->mask + 1) - (fifo->in - fifo->out);
	if (len > l)
		len = l;

	kfifo_copy_in(fifo, buf, len, fifo->in);
	fifo->in += len;

	return len;
}

static void kfifo_copy_out(struct kfifo *fifo, void *dst,
		unsigned int len, unsigned int off)
{
	unsigned int size = fifo->mask + 1;
	unsigned int l;

	off &= fifo->mask;

	l = min(len, size - off);

	ly_memcpy(dst, fifo->data + off, l);
	ly_memcpy(dst + l, fifo->data, len - l);
	
}

unsigned int kfifo_out(struct kfifo *fifo,
		void *buf, unsigned int len)
{
	unsigned int l;

	l = fifo->in - fifo->out;
	if (len > l)		
		len = l;

	kfifo_copy_out(fifo, buf, len, fifo->out);

	fifo->out += len;

	return len;
}



