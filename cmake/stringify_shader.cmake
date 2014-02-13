# Script required variables: ${SHADERNAME} ${INPUT_DIR} ${OUTPUT_DIR}
# message(STATUS "STRINGIFY SHADER CALLED")
# message(STATUS " SHADER " ${SHADERNAME})
# message(STATUS " INPUT_DIR " ${INPUT_DIR})
# message(STATUS " OUTPUT_DIR" ${OUTPUT_DIR})

# Extract all lines from ${SHADERNAME}
file(STRINGS ${INPUT_DIR}/${SHADERNAME}.glsl LINES)
# Write header of auto-generated shader
file(WRITE ${OUTPUT_DIR}/${SHADERNAME}.h
  "/* Generated file, DO NOT edit! */\n\n"
  "static char const* const ${SHADERNAME} = \n")    
# Write each line, but putting "\n" at the end
foreach(LINE ${LINES})
    string(REPLACE "\"" "\\\"" LINE "${LINE}")
    file(APPEND ${OUTPUT_DIR}/${SHADERNAME}.h "   \"${LINE}\\n\"\n")
endforeach(LINE)
# Close the file
file(APPEND ${OUTPUT_DIR}/${SHADERNAME}.h "   ;\n")
