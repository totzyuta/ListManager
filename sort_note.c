void fprintf_profile_column(FILE *fp, char *pre, struct profile *p, char *post, int column)
{
fprintf(fp, "%s", pre);

switch (column) {
  case 1: fprintf(fp, "%d", p->id);
  case 2: fprintf(fp, "%s", p->name);
  case 3: fprintf(fp, "%s", date_to_string(date, &p->birthday));
  case 4: fprintf // home
  case 5: fprintf // comment
  default: /* do nothing */
}

fprintf(fp, "%s", post);
}
