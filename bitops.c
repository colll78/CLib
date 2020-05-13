#define BITMAP_SIZE 5

/* 
* Set the priority_level'th bit in the bitmap to 1.
* 
*/
void set_bit(int priority_level, unsigned int* bitmap) {
    unsigned int mask = (1U << ((priority_level) % 32));
    unsigned int* p = ((unsigned int*)bitmap) + ((priority_level) / 32);
    *p |= mask;
}

/*
*  Finds the first set bit in the bitmap from right to left starting at bitmap[0]
*  and ending at the most significant bit in bitmap[BITMAPSIZE-1].
*/
int find_first_bit_naive_loop(unsigned int bitmap[BITMAP_SIZE]) {
    int total = 0;
    for (int i = 0; i < BITMAP_SIZE; i++) {
        unsigned int value = bitmap[i];
        if (value == 0) {
            total += sizeof(unsigned int) * 8;
            continue;
        }
        unsigned int pos = 0;
        while ( !(value & 1) ){
            value >>= 1;
            ++pos;
        }
        total += pos;
        return total;
    }
    return total;
}
