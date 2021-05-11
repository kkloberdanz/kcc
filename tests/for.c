int main(void) {
    int i = 0;
    int j = 0;
    for (i = 0; i < 10; i++) {
        j += i + 1;
        if (j == 17) {
            return 1;
        }
    }
    return 0;
}
