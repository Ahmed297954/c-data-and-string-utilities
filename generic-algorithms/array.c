/*
 * Author: Ahmed BOUAZZA
 * Description: Type-generic array utilities implementing predicate counting, linear/minimum search, byte-wise swapping (mem_swap), and generic selection sort.
 */
 
 #include "array.h"

size_t cond_count(const void *base, size_t nmemb, size_t size,
    bool (*cond)(const void *)) {
  size_t n = 0;
  const unsigned char *p = (const unsigned char *)base;
  const unsigned char *end = p + (nmemb * size);

  while (p < end) {
    if (cond((const void *)p)) {
      n++;
    }
    p += size;
  }
  return n;
}

void *cond_left_search(const void *base, size_t nmemb, size_t size,
    bool (*cond)(const void *)) {
  const unsigned char *p = (const unsigned char *)base;
  const unsigned char *end = p + (nmemb * size);

  while (p < end) {
    if (cond((const void *)p)) {
      return (void *)p;
    }
    p += size;
  }
  return nullptr;
}



static void mem_swap(void *p1, void *p2, size_t size) {
  unsigned char *q1 = (unsigned char *)p1;
  unsigned char *q2 = (unsigned char *)p2;
  
  // IB : 0 <= k && k <= size
  //   && les k premiers octets de p1 et p2 ont été échangés
  // QC : size - k
  for (size_t k = 0; k < size; ++k) {
    unsigned char temp = q1[k];
    q1[k] = q2[k];
    q2[k] = temp;
  }
}

void *min_left_search(const void *base, size_t nmemb, size_t size,
                      int (*compar)(const void *, const void *)) {
  if (nmemb == 0) {
    return nullptr;
  }
  const unsigned char *p = (const unsigned char *)base;
  size_t j = 0;
  
  // IB : 1 <= k && k <= nmemb
  //   && j == (indice de la première occurrence d'un composant du tableau
  //            dont l'indice est strictement inférieur à k et qui est minimum
  //            au sens de la fonction compar)
  // QC : nmemb - k
  for (size_t k = 1; k < nmemb; ++k) {
    if (compar(p + (k * size), p + (j * size)) < 0) {
      j = k;
    }
  }
  return (void *)(p + (j * size));
}

void select_sort(void *base, size_t nmemb, size_t size,
                 int (*compar)(const void *, const void *)) {
  unsigned char *p = (unsigned char *)base;
  
  // IB : 0 <= k && k <= nmemb
  //   && le sous-tableau base[0..k-1] est trié
  //   && tous les composants de base[0..k-1] sont inférieurs ou égaux 
  //      à tous les composants de base[k..nmemb-1]
  // QC : nmemb - k
  for (size_t k = 0; k < nmemb; ++k) {
    void *min_ptr = min_left_search(p + (k * size), nmemb - k, size, compar);
    if (min_ptr != p + (k * size)) {
      mem_swap(p + (k * size), min_ptr, size);
    }
  }
}


