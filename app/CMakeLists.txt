add_executable(cella_sim
    src/main.c
    src/controllore.c
)

# Collega l'eseguibile alla libreria commons e parsing, ereditando le loro include directory
target_link_libraries(cella_sim PRIVATE commons parsing)
