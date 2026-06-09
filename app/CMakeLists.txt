add_executable(cella_sim
    src/main.c
    src/controllore.c
)

# Collega l'eseguibile alla libreria commons, ereditando la include directory di commons
target_link_libraries(cella_sim PRIVATE commons)
