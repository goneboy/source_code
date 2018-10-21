#ifndef HEADER_H_
#define HEADER_H_

void StartUpFunc(void) __attribute__ ((constructor));
void CleanUpFunc(void) __attribute__ ((destructor));



#endif /* HEADER_H_ */