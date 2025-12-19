#include "./utility.h"
int max2(int a, int b) {
  return a >= b ? a : b;
}
int max3(int a, int b, int c) {
  return a >= b ? a >= c ? a : c : b >= c ? b : c;
}
int min2(int a, int b) {
  return a <= b ? a : b;
}
int min3(int a, int b, int c) {
  return a <= b ? a <= c ? a : c : b <= c ? b : c;
}
