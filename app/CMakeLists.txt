add_executable(cella_sim
    src/main.c
)

# Collega l'eseguibile alle librerie commons, model e parsing, ereditando le loro include directory
target_link_libraries(cella_sim PRIVATE commons model parsing controller io)
