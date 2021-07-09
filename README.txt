
▒▒▒▒▒▒▒▒▒▒▒▒  Introduccion teórica breve  ▒▒▒▒▒▒▒▒▒▒▒▒ 

    Una tabla de hash es un contenedor de un conjunto de datos
    que vienen caracterizados por tener una paridad de
    clave-valor para cada dato que se tiene. Como analogía
    directa para entenderlo se tiene un diccionario de
    significados, que contiene las palabras (que sería la
    clave o índice) y su definición conjunta (que sería su
    valor).

    Relacionado a lo anterior, una tabla de hash no puede 
    contener dos claves idénticas, pues se quiere que las 
    claves sean una forma única de representar un valor que 
    se encuentre alojado en la tabla.

    Para determinar la posición que le corresponde a cada
    clave en la tabla se utiliza una función de hash, que
    recibe la clave y devuelve algún valor que se pueda
    adaptar más adelante al tamaño actual que tenga la tabla.
    Como es de esperarse, independientemente de lo que haga la
    función, se van a tener casos en los que dicho valor
    devuelto se repita con otro obtenido anteriormente, es
    decir se asigne una posición repetida en la tabla a una
    clave nueva. Esto se denomina colisión, y se van a tener
    distintas formas de solucionar este problema, lo cual
    conlleva a tener distintos tipos de hash dependientes de
    cómo se trate el mismo. (Notar que NO es lo mismo
    una colisión y el intentar almacenar una clave repetida.
    Esto último es muy distinto ya que de por si no pueden
    haber claves repetidas en la tabla. Cuando se intenta
    almacenar claves repetidas se pueden tomar distintas
    convenciones para tratarlo, pero es un problema distinto).
    
    Dos de los tipos de tabla de hash son:
        -Hash abierto (de direccionamiento cerrado):
            Se denomina de direccionamiento cerrado porque,
            sin importar si hay una colisión o no, las claves
            se almacenan siempre en la posición de la tabla
            que les corresponde.
            Lo que se hace cuando hay colisión es utilizar
            algúna otra estructura, por lo general de tipo
            "encadenable" en cada posición de la tabla, para
            que si hay una colisión se pueda encadenar en la
            misma posición que le corresponde a pesar de que
            ya hubiese algo ahí.
        -Hash cerrado (de direccionamiento abierto):
            Se caracteriza por no hacer uso de estructuras
            auxiliares para almacenar, sino que se guardan
            los pares clave-valor en la misma tabla directamente.
            Se denomina de direccionamiento abierto porque si
            llega a haber una colisión, la nueva clave NO se 
            inserta en la posición que le corresponde, sino que
            se utilizan distintas estrategias para almacenarla 
            y poderla buscar más adelante de igual manera.
            Este tipo de tabla requiere que siempre el
            tamaño de la tabla sea mayor o igual a la cantidad de
            claves que hay actualmente dentro de la misma.
            Se suelen tomar diversas convenciones para tratar
            el caso de capacidad llena y de almacenamiento
            de colisiones.
            
    En general (para ambos tipos) es necesario establecer un
    punto de carga de la tabla en el que se tenga que rehashear,
    es decir, aumentar el tamaño de la tabla y volver a aplicar
    el hashing a todas las claves. Esto es para evitar una gran
    cantidad de colisiones y por ende evitar deterioro de 
    eficiencia en las operaciónes.
    (Para ver la convención tomada para esta implementación
    particular, ver sección Aclaraciones).

    Una gran cualidad que tiene es que, si bien los elementos
    que almacena no necesariamente están ordenados (que en la
    mayoría de los casos no lo están), al tener las claves
    como representación directa de un valor, se tiene una
    forma de acceder a dicho valor en la tabla simplemente
    conociendo su clave.
    Esto genera una ventaja en cuanto a complejidad de acceso
    a los valores con respecto de las otras estructuras de
    datos vistas en la materia, que por ejemplo siempre
    requerían de "recorrer" distintos elementos antes de poder
    llegar a uno en específico. (No siempre se va a lograr 
    obtener una complejidad deseada de O(1), pues las
    colisiones lo impiden y son inevitables sin importar la
    función de hashing que se use, pero aún así representa
    una ventaja bastante grande en dicha comparación).
        

    
▒▒▒▒▒▒▒▒▒▒▒▒  Explicación de implementación:  ▒▒▒▒▒▒▒▒▒▒▒▒ 

    Se omite:
        - Explicación de ciertas funciones o partes de las
        mismas que no la requieren (o que están explicadas en Aclaraciones).
        - Explicación de verificaciones visibles (de parámetros
        inválidos).

    ▒▒▒▒  hash.c  ▒▒▒▒

        --- hash_crear ---
            Para reservar la tabla, al haberla definido como vector
            dinámico, me convenía usar calloc así todas las casillas
            tenían en sus booleanos false y sus punteros iniciaizados
            a NULL.

        --- hash_insertar ---
            Si el factor de carga excede 0.75 (decisión explicada en Aclaraciones),
            se rehashea la tabla de hash.
            El rehasheo consiste en:
            Reemplazo la tabla al hash por una nueva con más almacenamiento y con
            todos los elementos que se tenían antes, pero vueltos a hashear.
            En resúmen (rehash):
                -Uso una copia del hash, principalmente para guardarme la tabla previa
                (de tal forma que si falla la reserva de memoria para la tabla ampliada,
                retorna con la misma tabla de antes así no se pierde el puntero antiguo).
                [Nota adicional: previo a insertar duplico el string clave para evitar
                posible uso inadecuado de las claves por parte del usuario (por ejemplo, el usuario decide
                insertar algo que debería tener clave CONSTANTE, sin embargo aún tiene control sobre casteos
                y puede llegar a modificar una clave que ya esté insertada, perjudicando la indexación) y
                tener claves completamente independizadas del mismo.]
                -Triplico la capacidad de la tabla. (Explicado en Aclaraciones).
                -Se reinsertan todos los datos que estaban en la tabla antigua a la 
                tabla nueva. Si dicho proceso falla se liberan todas las claves duplicadas que
                se hayan llegado a reinsertar y se libera la tabla que iba a hacer de reemplazo,
                por último se recupera la tabla antigua y devuelve una falla.
                Si anda todo bien, se liberan claves y tabla antiguas y ya se tiene la tabla nueva en
                el mismo hash.
            Luego de esto, se separa la inserción en casos:
                -Si hubo colisión, se busca y guarda la verdadera posición en donde debería insertarse, que
                es buscada de forma adyacente a partir de la posición inicial devuelta por el uso de la
                funcion de hash. Luego se guardan los datos en la posición encontrada.
                -Si se pidió inserción de clave repetida, se guarda la posición de dicha clave y se
                sobreescriben sus datos.
                -Si no pasó ninguno de los anteriores, simplemente se acaba de guardar perfectamente.


            

        ---  ---
            

        ---  ---
            
            

        ---  ---
            
        
        ---  ---
            

        ---  ---
            
        

▒▒▒▒▒▒▒▒▒▒▒▒  Aclaraciones:  ▒▒▒▒▒▒▒▒▒▒▒▒ 

    ▒▒▒▒  Estructura utilizada  ▒▒▒▒

        Se tiene en la estructura hash_t:
            1) La tabla, que es un vector dinámico de casillas.
            2) La capacidad actual de la tabla.
            3) La cantidad de casillas con elementos almancedos
            actualmente.
            4) El destructor de datos pasado por el usuario
            (en caso de tener uno).
        
        Se tiene en la estructura casilla_t:
            1) La clave de la casilla.
            2) El dato guardado correspondiente a su clave.
            3) Un flag que indica si la casilla tiene actualmente un dato guardado o no.
            4) Un flag que indica si la casilla tuvo alguna vez algún dato guardado
            pero se borró.


    ▒▒▒▒  Factor de carga elegido y criterios tomados  ▒▒▒▒

        Para esta implementación utilizaré un factor de carga
        de 0.75 como máximo para realizar el rehasheo, siendo
        'A' el factor de carga, 'N' el número de claves
        que actualmente se encuentran en la tabla y 'M'
        la capacidad actual de la tabla:
            A = N/M
        Esto teniendo en cuenta que:
            -Es un hash cerrado, con lo cual el numero de 
            claves almacenadas siempre debe ser menor o igual
            a la capacidad total de la tabla.
            -Considero que es un valor adecuado utilizar
            3/4 del espacio de la tabla según lo visto en
            clase para evitar rehashear muy seguido y seguir
            teniendo una cantidad manejable de colisiones.

        Además, tomé como factor de ampliación para rehashear
        el triple de capacidad de la tabla original.
        Esto debido a que al triplicar el tamaño de la tabla
        me aseguro de que, si se llega a tener que rehashear,
        se pueda llenar de forma asegurada como mínimo
        el doble de la tabla original sin que se provoque otro rehash.
        Considero que solo duplicar la tabla genera rehasheos bastante
        más constantes si la capacidad original es muy pequeña.

        Para la inserción en caso de colisión utilizaré
        el "probing lineal", es decir, si una clave colisiona
        la inserto en la siguiente posición adyacente que se 
        encuentre vacía en la tabla y que no haya tenido antes
        algo almacenado.

        Para el borrado usaré el campo 'dato_fue_eliminado' de cada
        casilla de la tabla como flag para determinar si
        hubo antes un dato almacenado en dicha posición o no.
        De esta forma evito tener que reinsertar casillas
        cuando se borran elementos que tenían colisiones 
        subyacentes previas. A pesar de eso cabe aclarar que
        se es consciente de que se puede llegar a deteriorar un
        poco la obtención de casillas cuando se hayan borrado
        muchos elementos, pues va a tardar más en encontrar
        posiciones vacías y que no hayan tenido nada antes almacenado.
        A mi parecer es razonable para poder evitar lo anteriormente
        mencionado.
        Precisamente para evitar el caso hipotético en el que la tabla
        quede "manchada" en todas sus posiciones con el flag activado,
        se tiene la función 'resetear_flags_de_borrado', que se llama
        cuando se quita un último dato de la tabla, es decir cuando
        tras borrar quede vacía, y así si se quiere seguir usando la 
        tabla se reestablece su eficiencia de inserción y búsqueda.


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
        ser un tanto criticada (ver:
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

        (Nota: Cabe destacar que a pesar de lo encontrado acerca del uso de la suma de todos
        los caracteres del string, no considero que sea una mala función de hash del todo
        simplemente por leer esas dos críticas, creería que tambien está bien usarla para
        lo que se quiere actualmente, no traería problema en este contexto).


    ▒▒▒▒    ▒▒▒▒

        


    ▒▒▒▒    ▒▒▒▒

        
                                               

    ▒▒▒▒    ▒▒▒▒

       