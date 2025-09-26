int main() {

  int i, n=10;

  // initialize first and second terms
  int t1 = 0, t2 = 1;

  // initialize the next term (3rd term)
  int nextTerm = t1 + t2;

  // calculate fibonacci sequence without printing
  for (i = 3; i <= n; ++i) {
    t1 = t2;
    t2 = nextTerm;
    nextTerm = t1 + t2;
  }

  // Store result in a global variable to make it visible
  volatile int result = nextTerm;

  // Try writing to a fixed memory address that might be visible
  volatile int *output = (volatile int *)0x1000;
  *output = nextTerm;

  return nextTerm;  // return the final fibonacci number
}