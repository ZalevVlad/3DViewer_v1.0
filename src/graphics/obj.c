#include "obj.h"

s21_obj create_obj() {
    s21_obj obj;
    obj.facets.facets = NULL;
    obj.facets.len = 0;
    obj.vertexes.vertexes = NULL;
    obj.vertexes.len = 0;
    return obj;
}

void count_objs(FILE* file, s21_obj* obj) {
    // TODO: Reading file by lines
    fseek(file, 0, SEEK_SET);
    int vcounter = 0, fcounter = 0;
    char line[MAX_SIZE];

    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) > 1) {
            if (strncmp(line, "v ", 2) == 0) {
                vcounter++;
            } else if (strncmp(line, "f ", 2) == 0) {
                fcounter++;
            }
        }
    }

    obj->vertexes.len = vcounter;
    obj->facets.len = fcounter;
}

int count_nums(const char* str) {
    int num, counter = 0, scanned;
    while (sscanf(str, "%d%n", &num, &scanned) == 1) {
        counter++;
        str += scanned;
        while (!((*str) == '\0' || (*str) == ' ' || (*str) == '\n')) {
            str++;
        }
    }
    return counter;
}

#define OBJ_MIN_VERTECIES_NUM 2
int scan_facet(const char* str, s21_facet* facet, int vertecies_scanned) {
    int res = OK;
    int number;
    int nnums = count_nums(str);
    if (nnums >= OBJ_MIN_VERTECIES_NUM) {
        facet->numbers = calloc(nnums, sizeof(int));
        facet->len = nnums;
        int scanned;
        for (int i = 0; i < nnums; i++) {
            sscanf(str, "%d%n", &number, &scanned);
            if (number > 0) {
                facet->numbers[i] = number;
            } else {
                facet->numbers[i] = number + vertecies_scanned + 1;
            }
            str += scanned;
            while (!((*str) == '\0' || (*str) == ' ' || (*str) == '\n')) {
                str++;
            }
        }
    } else {
        res = ERR;
    }
    return res;
}

int scanf_objs(FILE* file, s21_obj* obj) {
    int res = OK;
    fseek(file, 0, SEEK_SET);
    obj->vertexes.vertexes = calloc(obj->vertexes.len, sizeof(s21_vertex));
    obj->facets.facets = calloc(obj->facets.len, sizeof(s21_facet));

    char line[MAX_SIZE];
    int vcounter = 0, fcounter = 0;
    while (fgets(line, MAX_SIZE, file) && res == OK) {
        if (strlen(line) > 1) {
            if (strncmp(line, "v ", 2) == 0) {
                int sscanf_res = sscanf(line, "v %lf %lf %lf",
                                        &obj->vertexes.vertexes[vcounter].x,
                                        &obj->vertexes.vertexes[vcounter].y,
                                        &obj->vertexes.vertexes[vcounter].z);
                if (sscanf_res != 3) {
                    res = ERR;
                }
                obj->vertexes.vertexes[vcounter].w = 1.;  // Мало ли пригодится
                vcounter++;
            } else if (strncmp(line, "f ", 2) == 0) {
                res = scan_facet(line + 1, &obj->facets.facets[fcounter],
                                 vcounter);
                fcounter++;
            }
        }
    }
    return res;
}

// Check indicies
int s21obj_check_indicies(s21_obj* obj) {
    int res = OK;
    int current, max=obj->vertexes.len;
    for(int i=0; i<obj->facets.len && res==OK;i++){
        for(int j=0; j<obj->facets.facets[i].len&& res==OK;j++){
            current = obj->facets.facets[i].numbers[j];
            if(current<1 || current>max){res=ERR;}
        }
    }
    return res;
}

int parse_obj(const char* filename, s21_obj* obj) {
    int res = 0;
    FILE* file = fopen(filename, "r");
    if (file) {
        count_objs(file, obj);
        res = scanf_objs(file, obj);
        if (res == OK) {
            res = s21obj_check_indicies(obj);
        }
        fclose(file);
    } else {
        res = FILE_OPEN_ERR;
        fprintf(stderr, "FILE: %s : doesnt open\n", filename);
    }
    return res;
}

void free_obj(s21_obj* obj) {
    if (obj) {
        if (obj->vertexes.vertexes) {
            free(obj->vertexes.vertexes);
            obj->vertexes.vertexes = NULL;
        }
        if (obj->facets.facets) {
            for (int i = 0; i < obj->facets.len; i++) {
                free(obj->facets.facets[i].numbers);
            }
            free(obj->facets.facets);
            obj->facets.facets = NULL;
        }
    }
}
