#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>

typedef struct files_and_dirs {
  size_t dirs;
  size_t files;
} files_and_dirs_t;

typedef struct desc {
  char *name;
  int number_dir;
  struct desc *next; //pointer to next 
} desc_t;

int walk(const char* direct, const char* prefix, files_and_dirs_t *files_and_dirs) {
  desc_t *parent = NULL, *curr, *iter;
  size_t size = 0, index;

  struct dirent *file_dirent;
  DIR *dir_handle;

  char *full_path, *seg, *point, *next_pre;

  dir_handle = opendir(direct); //open directory
  if (!dir_handle) {
    fprintf(stderr, "Cannot open directory \"%s\"\n", direct);
    return -1;
  }

  files_and_dirs->dirs++;

  while ((file_dirent = readdir(dir_handle)) != NULL) {
    if (file_dirent->d_name[0] == '.') {
      continue;
    }

    curr = malloc(sizeof(desc_t));
    //allocating memory for file object
    curr->name = strcpy(malloc(strlen(file_dirent->d_name) + 1), file_dirent->d_name);
    curr->number_dir = file_dirent->d_type == DT_DIR;
    curr->next = NULL;

    if (parent == NULL) {
      parent = curr;
    } else if (strcmp(curr->name, parent->name) < 0) {
      curr->next = parent;
      parent = curr; //DFS
    } else {
      for (iter = parent; iter->next && strcmp(curr->name, iter->next->name) > 0; iter = iter->next);

      curr->next = iter->next;
      iter->next = curr;
    }

    size++;
  }

  closedir(dir_handle);
  if (!parent) {
    return 0;
  } //reach end

  for (index = 0; index < size; index++) {
    point = "| _ _ ";
    seg = "   ";

    printf("%s%s%s\n", prefix, point, parent->name);
    //printf("This is parent");

    if (parent->number_dir) {
      full_path = malloc(strlen(direct) + strlen(parent->name) + 2);
      sprintf(full_path, "%s/%s", direct, parent->name);
        //sprintf("parent->name");


      next_pre = malloc(strlen(prefix) + strlen(seg) + 1);
      sprintf(next_pre, "%s%s", prefix, seg);

      walk(full_path, next_pre, files_and_dirs);
      //sprintf("*full_path");
      free(full_path);
      free(next_pre);

      
    } else {
      files_and_dirs->files++;
    }

    curr = parent;
    parent = parent->next;

    free(curr->name);
    free(curr);

    //free memory
  }

  return 0;
}

int main(int argc, char* argv[]){
    char* direct=(argc>2)?argv[2]:".";
    printf("%s\n", direct);

    if (strcmp(argv[1],"-d")==0){
        files_and_dirs_t lol1 = {0, 0};
        walk(direct, "", &lol1);

        printf("\n%zu directories, %zu files\n",
            lol1.dirs ? lol1.dirs - 1 : 0, lol1.files);
    }
    else if (strcmp(argv[1],"-b")==0){
        files_and_dirs_t lol2 = {0, 0};
        walk(direct, "", &lol2);

        printf("\n%zu directories, %zu files\n",
            lol2.dirs ? lol2.dirs - 1 : 0, lol2.files);
    }
    return 0;
}