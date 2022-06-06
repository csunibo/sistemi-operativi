#include <stdio.h>
 
struct elem {
  int val;
  struct elem *next;
};
 
struct elem *head = NULL;
 
struct elem *rinsert(struct elem *new, struct elem *head) {
  if (head == NULL || new->val < head->val) {
    new->next = head;
    return new;
  } else {
    head->next = rinsert(new, head->next);
    return head;
  }
}
 
struct elem *iinsert(struct elem *new, struct elem *head) {
  struct elem **pnext;
  for (pnext = &head;
      *pnext != NULL && new->val > (*pnext)->val;
      pnext = &((*pnext)->next))
    ;
  new->next = *pnext;
  *pnext = new;
  return head;
}
 
void rprint(struct elem *this) {
  if (this) {
    printf("%d ",this->val);
    rprint(this->next);
  }
}
 
void iprint(struct elem *this) {
  for ( ; this != NULL; this = this->next)
    printf("%d ",this->val);
}
 
struct elem test[]={{5},{3},{9},{1},{7}};
#define NELEM (sizeof(test) / sizeof(struct elem))
 
int main(int argc, char *argv[]) {
  int i;
  for (i = 0; i < NELEM; i++)
    head = rinsert(&test[i], head);
  rprint(head);
  printf("\n");
  iprint(head);
  printf("\n");
  head = NULL;
  for (i = 0; i < NELEM; i++)
    head = iinsert(&test[i], head);
  rprint(head);
  printf("\n");
  iprint(head);
  printf("\n");
}
