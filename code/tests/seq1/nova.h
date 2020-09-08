#ifndef NOVA_CHECKPOINT_H
#define NOVA_CHECKPOINT_H

int novaCheckpoint();
int novaWriteData(int fd, unsigned int offset, unsigned int size);

#endif 