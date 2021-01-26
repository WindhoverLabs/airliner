# Dimensions of matrix 1 rows
ROWM1 = 10;
# Dimensions of matrix 1 cols
COLM1 = 2;
# Dimensions of matrix 2 rows
ROWM2 = 2;
# Dimensions of matrix 2 cols
COLM2 = 1;
# Max random value of generated matrices
MAX = 1;
# Min random value of generated matrices 
MIN = -1;

# Random matrix A
A = MAX + (MIN-MAX)*rand(ROWM1, COLM1);
# Random matrix B
B = MAX + (MIN-MAX)*rand(ROWM2, COLM2);
# Result
R = A * B;

filename = "auto_generated_unit_test.txt";
fid = fopen (filename, "w");

# Matrix A
fprintf(fid, "void Test_Matrix%dF%d_Mult_%dF_Random(void)\n", ROWM1, COLM1, ROWM2);
fprintf(fid, "{\n");
fprintf(fid, "    int i, j = 0;\n\n");
fprintf(fid, "    math::Matrix%dF%d A (\n", ROWM1, COLM1);
fprintf(fid, "        {");
for i = 1:ROWM1
        for j = 1:COLM1
            fprintf(fid, "%f", A(i,j));
            if(j < COLM1)
                fprintf(fid, ", ");
            elseif(j == COLM1)
                if(i < ROWM1)
                    fprintf(fid, "},");
                elseif(i == ROWM1)
                    fprintf(fid, "}");
                endif
            endif
        endfor
        fprintf(fid, "\n");
        if(i < ROWM1)
            fprintf(fid, "        {");
        endif
endfor
fprintf(fid, "    );\n\n");

# Vector B
fprintf(fid, "    math::Vector%dF B (\n", ROWM2);
fprintf(fid, "        {");
for i = 1:ROWM2
        for j = 1:COLM2
            fprintf(fid, "%f", B(i,j));
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
            fprintf(fid, "%f", R(i,j));
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
fprintf(fid, "    result = A * B;\n\n");
fprintf(fid, "    for(i = 0; i < %d; i++)\n", ROWM1);
fprintf(fid, "    {\n");
fprintf(fid, "        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, \"fabs(result[i] - expected[i]) < 0.00001f\");\n\n")
fprintf(fid, "    }\n");
fprintf(fid, "}\n");
fclose(fid);
