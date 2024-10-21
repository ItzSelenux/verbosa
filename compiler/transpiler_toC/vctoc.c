#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// Function to handle @include
void transpile_include(FILE *output, const char *line) {
    char type[10], items[256];
    if (sscanf(line, "@include (%[^)]) [%[^]]];", type, items) == 2) {
        char *token = strtok(items, ", ");
        while (token != NULL) {
            if (strcmp(type, "header") == 0) {
                fprintf(output, "#include <%s.h>\n", token);
            } else if (strcmp(type, "file") == 0) {
                fprintf(output, "#include \"%s\"\n", token);
            }
            token = strtok(NULL, ", ");
        }
    }
}

// Function to handle @declare
void transpile_declare(FILE *output, const char *line) {
    char var_type[10], var_name[50], value[50];
    if (sscanf(line, "@declare (var.%[^)]) [%[^=] = %[^]]];", var_type, var_name, value) == 3) {
        if (strcmp(var_type, "int") == 0) {
            fprintf(output, "int %s = %s;\n", var_name, value);
        } else if (strcmp(var_type, "char") == 0) {
            fprintf(output, "char %s = '%s';\n", var_name, value);
        } else if (strcmp(var_type, "str") == 0) {
            fprintf(output, "char* %s = \"%s\";\n", var_name, value);
        }
    }
}

// Function to handle @modify
void transpile_modify(FILE *output, const char *line) {
    char var_name[50], value[50];
    if (sscanf(line, "@modify (%[^)]) [%[^=] = %[^]]];", var_name, var_name, value) == 3) {
        fprintf(output, "%s = %s;\n", var_name, value);
    }
}

// Function to handle @control (if/else/elseif)
void transpile_control(FILE *output, const char *line) {
    char control_type[10], condition[256];
    if (sscanf(line, "@control %*s (%[^)]) [%[^]]]:", control_type, condition) == 2) {
        if (strcmp(control_type, "if") == 0) {
            fprintf(output, "if (%s) {\n", condition);
        } else if (strcmp(control_type, "elseif") == 0) {
            fprintf(output, "} else if (%s) {\n", condition);
        } else if (strcmp(control_type, "else") == 0) {
            fprintf(output, "} else {\n");
        }
    }
}

// Function to handle @function
void transpile_function(FILE *output, const char *line) {
    char func_name[50], return_type[10], args[256];
    if (sscanf(line, "@function %s (%[^)]) [%[^]]]:", func_name, return_type, args) == 3) {
        if (strcmp(return_type, "int") == 0) {
            fprintf(output, "int %s(%s) {\n", func_name, strcmp(args, "h.vc.argv") == 0 ? "int argc, char* argv[]" : args);
        }
    }
}

// Function to handle @call
void transpile_call(FILE *output, const char *line) {
    char func_type[50], func_name[50], args[256];
    if (sscanf(line, "@call (%[^)]) [%[^]]];", func_type, args) == 2) {
        if (strcmp(func_type, "file.print") == 0) {
            fprintf(output, "printf(%s);\n", args);
        } else if (strcmp(func_type, "file.return") == 0) {
            fprintf(output, "return %s;\n", args);
        }
    }
}

// Main transpiler function
void transpile(FILE *input, FILE *output) {
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), input)) {
        if (strstr(line, "@include") != NULL) {
            transpile_include(output, line);
        } else if (strstr(line, "@declare") != NULL) {
            transpile_declare(output, line);
        } else if (strstr(line, "@modify") != NULL) {
            transpile_modify(output, line);
        } else if (strstr(line, "@control") != NULL) {
            transpile_control(output, line);
        } else if (strstr(line, "@function") != NULL) {
            transpile_function(output, line);
        } else if (strstr(line, "@call") != NULL) {
            transpile_call(output, line);
        } else if (strcmp(line, "}\n") == 0) {
            fprintf(output, "}\n");
        }
    }
}

// Main function to handle file input/output
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        perror("Error opening input file");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (!output) {
        perror("Error opening output file");
        fclose(input);
        return 1;
    }

    transpile(input, output);

    fclose(input);
    fclose(output);

    printf("Transpilation complete! Output written to %s\n", argv[2]);
    return 0;
}
