
▒▒▒▒▒▒▒▒▒▒▒▒  Introduccion teórica breve  ▒▒▒▒▒▒▒▒▒▒▒▒ 

    a
        

    
▒▒▒▒▒▒▒▒▒▒▒▒  Explicación de implementación:  ▒▒▒▒▒▒▒▒▒▒▒▒ 

    Se omite:
        - Explicación de ciertas funciones o partes de las mismas que no la requieren.
        - Explicación de verificaciones visibles (de parámetros
        inválidos).

    ▒▒▒▒  hash.c  ▒▒▒▒

        ---  ---
            

        ---  ---
            

        ---  ---
            

        ---  ---
            
            

        ---  ---
            
        
        ---  ---
            

        ---  ---
            
        

▒▒▒▒▒▒▒▒▒▒▒▒  Aclaraciones:  ▒▒▒▒▒▒▒▒▒▒▒▒ 

    ▒▒▒▒    ▒▒▒▒

        

    ▒▒▒▒  Sobre la elección de la función de hash  ▒▒▒▒

        Claramente no existe tal cosa como una función de hash
        perfecta, y existen infinitas funciones de hash posibles, hay
        unas cuantas conocidas que son bastante eficientes, a pesar de que
        las mismas dependen de lo que se quiera tratar.
        Para no adentrarse mucho en el tema de optimización, que no es
        fundamental para lo que se quiere, decidí elegir una función de 
        hash como la encontrada en el siguiente enlace:
            "https://stackoverflow.com/questions/2624192/good-hash-function-for-strings"
        Si bien la pregunta se plantea para uso en Java, tal y como indica
        la persona que realizó la pregunta, eso no debería afectar ya que 
        el comportamiento de hashing es el mismo independientemente de la
        diferencia de lenguaje. 
        Decidí en específico esa forma de hashing porque adjunto a la
        respuesta principal se encuentra un comentario con un enlace
            ("https://computinglife.wordpress.com/2008/11/20/why-do-hash-functions-use-prime-numbers/")
        a modo explicar de manera bastante simple de por qué se usan los
        números primos para ir generando el valor resultante.
        En la clase práctica implementamos un hash abierto, y como resultado de la
        función de hash habíamos utilizado la suma del
        valor ASCII correspondiente a cada letra del string.

        Mientras buscaba entre implementaciones de funciones de hashing 
        conocidas me encontré con una sugerencia de ese estílo y resulta
        ser un tanto ineficiente (ver:
            "https://stackoverflow.com/questions/7666509/hash-function-for-string";
            "http://www.cse.yorku.ca/~oz/hash.html", sección: 'lose lose')
        , pero nuevamente no adentraré mucho
        para evitar alargarme tanto en tema de optimización que no viene
        tanto al caso.

        Concluí en base a todo lo anterior que para este caso basta con 
        utilizar el concepto de la "ventaja" de multiplicar por numeros primos,
        pero simplemente usando unos más pequeños que 31 (5 inicial y 3 como
        factor, ambos primos), por simple elección arbitraria y para que el
        resultado de hashear no resulte en valores exageradamente grandes a mi parecer.


    ▒▒▒▒    ▒▒▒▒

        


    ▒▒▒▒    ▒▒▒▒

        
                                               

    ▒▒▒▒    ▒▒▒▒

        








        
