# Dimensions of matrix 1 rows
ROWM1 = 1;
# Dimensions of matrix 1 cols
COLM1 = 1;
# Dimensions of matrix 2 rows
ROWM2 = ROWM1;
# Dimensions of matrix 2 cols
COLM2 = 1;
# Max random value of generated matrices
MAX = 1;
# Min random value of generated matrices 
MIN = -1;

# Random matrix A
A = MAX + (MIN-MAX)*rand(ROWM1);
# Random matrix B
B = MAX + (MIN-MAX)*rand(ROWM2);
# Result
R = A + B;

filename = "auto_generated_unit_test.txt";
fid = fopen (filename, "w");

# Matrix A
fprintf(fid, "void Test_Vector%dF_Add_%dF_Random(void)\n", ROWM1, ROWM2);
fprintf(fid, "{\n");
fprintf(fid, "    int i, j = 0;\n\n");
fprintf(fid, "    math::Vector%dF A (\n", ROWM1);
fprintf(fid, "        {");
for i = 1:ROWM2
        for j = 1:COLM2
            fprintf(fid, "%f", A(i));
            if(j < COLM2)
                fprintf(fid, ", ");
            elseif(j == COLM2)
                if(i < ROWM2)
                    fprintf(fid, ",");
                elseif(i == ROWM2)
                    fprintf(fid, "}");
                endif
            endif
        endfor
endfor
fprintf(fid, "\n    );\n\n");

# Matrix B
fprintf(fid, "    math::Vector%dF B (\n", ROWM2);
fprintf(fid, "        {");
for i = 1:ROWM2
        for j = 1:COLM2
            fprintf(fid, "%f", B(i));
            if(j < COLM2)
                fprintf(fid, ", ");
            elseif(j == COLM2)
                if(i < ROWM2)
                    fprintf(fid, ",");
                elseif(i == ROWM2)
                    fprintf(fid, "}");
                endif
            endif
        endfor
endfor
fprintf(fid, "\n    );\n\n");

# Expected matrix
fprintf(fid, "    math::Vector%dF expected (\n", ROWM1);
fprintf(fid, "        {");
for i = 1:ROWM1
        for j = 1:COLM2
            fprintf(fid, "%f", R(i));
            if(j < COLM2)
                fprintf(fid, ", ");
            elseif(j == COLM2)
                if(i < ROWM1)
                    fprintf(fid, ",");
                elseif(i == ROWM1)
                    fprintf(fid, "}");
                endif
            endif
        endfor
endfor
fprintf(fid, "\n    );\n\n");

# Verify results
fprintf(fid, "    math::Vector%dF result;\n", ROWM1);
fprintf(fid, "    result.Zero();\n\n");
fprintf(fid, "    result = A + B;\n\n");
fprintf(fid, "    for(i = 0; i < %d; i++)\n", ROWM1);
fprintf(fid, "    {\n");
fprintf(fid, "        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, \"fabs(result[i] - expected[i]) < 0.00001f\");\n\n")

fprintf(fid, "    }\n");
fprintf(fid, "}\n");
fclose(fid);