#ifndef _OB_SHADER_H_
#define _OB_SHADER_H_
// OpenGL/glfw includes
#include <GL/glew.h>
#include <GL/glfw.h>

// standard includes
#include <stdio.h>
#include <stdlib.h>

int loadShader(GLchar *dest, const GLchar *file, const GLuint f_len) {
    FILE *fp = fopen(file, "r");

    // check if the file is open
    if (!fp) {
        fprintf(stderr, "Could not open file for reading: %s\n", file);
        return EXIT_FAILURE;
    }

    // check that the file length is valid
    if (f_len <= 0) {
        fprintf(stderr, "File is of 0 length or has invalid size: %s\n    with size %d\n", file, f_len);
        return EXIT_FAILURE;
    }

    // read the file into dest
    if (f_len != fread(dest, sizeof(GLchar), f_len, fp)) {
        fprintf(stderr, "Could not read %d bytes of file %s: Destination string too small or file invalid.\n", f_len, file);
        return EXIT_FAILURE;
    }

    // ensure the string is null terminated and return
    dest[f_len] = '\0';
    return EXIT_SUCCESS;
}

// returns length of the file
unsigned int fileLength(const GLchar *file) {
    // file length
    int f_len = 0;

    // file pointer
    FILE *fp = fopen(file, "r");

    // check if the file is open
    if (!fp) {
        fprintf(stderr, "Could not open file for reading: %s\n", file);
        return 1;
    }

    // attempt to get the file length
    if (fseek(fp, 0, SEEK_END)) {
        fprintf(stderr, "Failed to read EOF in file: %s\n", file);
        return 1;
    }

    // get the file length
    f_len = ftell(fp);

    // check that the file length is valid
    if (f_len <= 0) {
        fprintf(stderr, "File is of 0 length or has invalid size: %s\n    with size %d\n", file, f_len);
        return 1;
    }
    
    // close the file and return
    fclose(fp);
    return f_len;
}

/* FUNCTION: OB_compileShader(GLchar*, const GLchar*)
 * PURPOSE: Load the file from the path given by file into dest and compile
        the shader.
 * RETURN: Returns EXIT_SUCCESS after successful execution, gives
 *      EXIT_FAILURE in case of failure of fopen, fseek, ftell, or fread.
 *      
 *
 * ------PARAMETERS------
 * dest (GLchar*): Destination char* to load the file contents into, will
 *      contain the entire file after successful execution.  Must be of
 *      size (file_length + 1) so as to accomodate a null character.
 * file (GLchar*): File path for the shader to be loaded.  Can be
 *      either absolute or relative.
 * GLuint type: 
 */
int OB_compileShader(const GLchar *file, GLuint type, GLuint *shader) {
    GLchar src[fileLength(file) + 1];

    // attempt to load the file
    if(loadShader(&src, file, sizeof(src) - 1) == 0) {
        // attempt to create the shader
        *shader = glCreateShader(type);

        // set the shader source
        glShaderSource(*shader, 1, &src, &sizeof(src) - 1);

        // compile the shader
        glCompileShader(*shader);
        return EXIT_SUCCESS;
    }
    fprintf(stderr, "Failed to load shader.");
    return EXIT_FAILURE;
}
#endif
