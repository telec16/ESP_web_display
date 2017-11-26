#ifndef TEXTER_H
#define TEXTER_H

void writeWord(byte addr, byte data);
void writeWord(byte addr, byte data, boolean hold);
void init_matrix(byte intensity);
void writeMsg(const char msg[], int defilDelay);
void writeLetter(char letter);
void shiftBuffer(char nextLetter, int offset);
void applyBuffer();
void changeIntensity(int intensity);

#endif
