# Autores:
#	Luigi Quero, 	   C.I: 30.009.785	      
#	Sebastian Piñango, C.I: 29.947.488

# Algoritmo que calcula la serie Fibonachi
.data
	array:.word 6 1 9 7 3 8 5 2 4 10    	      # Array de numeros
	array_size: .word  10               	      # Tamanño el array de numeros
	space:.asciiz  " "                   	      # mensaje a imprimir en pantalla
	mensaje_1: .asciiz  "\nVector:" 	      # mensaje a imprimir en pantalla
.text
	main:   
		la $a0, array		# Se guarda la direccion de array en $a0
		lw $a1, array_size	# Se guarda el tamaño del array en $a1
	
		jal imprimir		# Imprimir el vector

		la $a1, array		# Se guarda la direccion de array en $a0
		lw $a0, array_size	# Se guarda el tamaño del array en $a1
	
		jal ordenamiento_burbuja	# Se llama a ordenamiento_burbuja para ordenar el vector
	
		la $a0, array		# Se guarda la direccion de array en $a0
		lw $a1, array_size	# Se guarda el tamaño del array en $a1
	
		jal imprimir		# Imprimir el vector

		li   $v0, 10            # Se guarda el valor 10 en $v0 para indicarle a syscall la operacion a relizar
		syscall                 # Finaliza el programa
		
		
# Procedimiento que imprime los elementos del vector
	imprimir:
		add  $t0, $zero, $a0  # Se lee en $t0 la direccion del vector
      		add  $t1, $zero, $a1  # Se lee el tamaño del vector en $t1
     		
     		la   $a0, mensaje_1   # Se lee la direccion de mensaje_2 en $a0
   		li   $v0, 4           # Se guarda el valor 4 en $v0 para indicarle a syscall la operacion a relizar
    		syscall               # Se imprime una cadena en pantalla
      
	salida:  
		lw   $a0, 0($t0)      # Se lee el numero actual del vector a imprimir
    		li   $v0, 1           # Se guarda el valor 1 en $v0 para indicarle a syscall la operacion a relizar
      		syscall               # Se imprime el numero en pantalla
      
		la   $a0, space       # Se lee la direccion de space en $a0
      		li   $v0, 4           # Se guarda el valor 4 en $v0 para indicarle a syscall la operacion a relizar
      		syscall               # Se imprime una cadena en pantalla
      
      		addi $t0, $t0, 4      # Se incremente en 1 la direccion del vector
      		addi $t1, $t1, -1     # Se disminuye el tamaño del vector en 1
      		bgtz $t1, salida         # Se verifica si se ha imprimido todos los numeros
      
      		jr   $ra              # Se regresa al programa principal


# Procedimiento que ordena el vector
	ordenamiento_burbuja:
		addi $sp, $sp, -12 	# Se reserva tres espacios en la pila de memoria
		sw $s0, 8($sp)		# Se repaldan los registros $s0, $s1, $s2
		sw $s1, 4($sp)		 
		sw $s2, 0($sp)		 
		
		sll $s2, $a0, 2		# Se guarda en $s2 el tamaño del vector * 4 para verificar si se llego al final
		add $s0, $zero, $zero	# Se usa $s0 como iterador i del vector
		addi $s2, $s2, -4	# Se disminuye $s2 para evitar consultar una posicion erronea
		
	ciclo_1:
		beq $s0, $s2, salir_2	# Si el iterador llego al final, se sale del algoritmo
		add $s1, $zero, $zero	# Se inicializa $s1 en 0 como iterador j
		
	ciclo_2:
		beq $s1, $s2, salir_1	# Si j llego al final del vector se salde del ciclo secundario
		add $t0, $a1, $s1	# Se calcula la direccion de v[j] 
		lw $t1, 0($t0)		# Se cargar v[j] en $t1 
		lw $t2, 4($t0)		# Se carga v[j+1] en $t2 
		
		slt $t3, $t2, $t1	# Se verifica $t3 = 1 si v[j+1] < j[j] sino entonces $t3 = 0
		beq $t3, $zero, no_swap # Si $t3 = 0 no se realia el intercambio
		sw $t2, 0($t0)		# Se intercambian v[j] y v[j+1]
		sw $t1, 4($t0)		
		
	no_swap:
		addi $s1, $s1, 4	# j = j + 1
		j ciclo_2		# Se regresa al bucle interior
		
	salir_1:
		addi $s0, $s0, 4	# i = i + 1
		j ciclo_1		# Se regersa al bucle principal
		
	salir_2:
		lw $s2, 0($sp)		# Se restauran los valores de los registros respaldados en la pila de memoria
		lw $s1, 4($sp)		
		lw $s0, 8($sp)		
		addi $sp, $sp, 12	# Se eliminan los espacios reservados en la pila
		
		jr $ra			# Se regresa al programa principal
