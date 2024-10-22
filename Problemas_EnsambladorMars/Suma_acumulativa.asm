# Autores:
#	Luigi Quero, 	   C.I: 30.009.785	      
#	Sebastian Piñango, C.I: 29.947.488

.data
	mensaje_1: .asciiz "Ingrese cualquier numero mayor o igual 0 para calcular su suma acumulativa: "
	mensaje_2: .asciiz "Su suma acumilativa es: "
.text
	main:
		la $a0, mensaje_1     # Se guarda la direccion de mensaje_1 en $a0
		li $v0, 4	      # Se guarda el valor 4 en $v0 para indicarle a syscall la operacion a relizar
		syscall		      # Se imprime una cadena en pantalla
		
		li   $v0, 5           # Se guarda el valor 5 en $v0 para indicarle a syscall la operacion a relizar
		syscall               # Se lee un numero por pantalla y se guarda en $v0
		
		blt  $v0, $zero, main # Se verifica que el numero sea mayor o igual 0, sino se repite hasta que este sea mayor a 0
		add  $s0, $v0, $zero  # Se guarda el numero leido por pantalla en $s0
	
		add  $a0, $s0, $zero  # Se guarda en $a0 el numero leido por pantalla, para pasarlo como parametro a la funcion suma
		jal suma	      # Se llama a la funcion suma
		add  $s0, $v0, $zero  # Se guarda el valor retornado por suma en $s0
	
		la $a0, mensaje_2     # Se guarda la direccion de mensaje_2 en $a0
		li $v0, 4	      # Se guarda el valor 4 en $v0 para indicarle a syscall la operacion a relizar
		syscall		      # Se imprime una cadena en pantalla
	
		add $a0, $s0, $zero   # Se guarda el valor retornado por la suma en $a0 para imprimirlo en pantalla
		li $v0, 1	      # Se guarda el valor 1 en $v0 para indicarle a syscall la operacion a relizar
		syscall		      # Imprime el resultado de la funcion suma
	
		li   $v0, 10          # Se guarda el valor 10 en $v0 para indicarle a syscall la operacion a relizar
		syscall               # Finaliza el programa
		
		
# Funcion que calcula la suma acumilativa de un numero
	suma:    
		beq $a0, $zero, salida # Si n <= 0 salta a salida
		add $a1, $a1, $a0      # Se suma n a el acumulado
		addi $a0, $a0, -1      # Se disminuye n en 1
		
		j suma		       # Se salta de regreso a suma
		
	salida:
		add $v0, $a1, $zero    # Se retorna el valor acumulado
		
		jr $ra 		       # Se regresa a la funcion principal

