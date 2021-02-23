# 13519214 - Build OS then run with bochs
sudo make clean;
make all &&
sudo bochs -f src/if2230.config;
