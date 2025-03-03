#ifndef ATM_UTILS_H
#define ATM_UTILS_H

#define ATM_ERROR(fmt, ...)                                 \
  do {                                                      \
      fprintf(stderr, "ERROR: " fmt "\n", ##__VA_ARGS__);   \
      exit(EXIT_FAILURE);                                   \
  } while (0)

#define ATM_LOG(fmt, ...) \
  fprintf(stdout, "ATMTool: " fmt "\n", ##__VA_ARGS__);


#endif