/*
2-Proporcionar, para cada contrato comercial, 
las llamadas que excedan de 10
minutos de duración, en un rango de fechas 
dado (del 01/10/2020 al 31/10/2020)
*/
select id_contrato, 
		tipo_llamada,
		numero_destino,
		fecha,
		duracion,
		id_destino
		from llamada
	where (fecha >= '2020/10/01' and fecha <= '2020/10/31') and 
			(duracion > '00:10:00') and
		(id_contrato in (select numero_telefono from contrato
		where tipo_telefono = 'Comercial') );

