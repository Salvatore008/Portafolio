/*
1-Proporcionar aquellos contratos residenciales que 
sólo están afiliados al servicio “Teleamigo”
*/
select id_servicio, id_contrato from tiene_servicio
	where id_servicio = 'STA' and 
		id_contrato in (select numero_telefono from contrato
		where tipo_telefono = 'Residencial');