/*
4-Proporcionar, para cada suscriptor, las 
llamadas no locales a teléfonos
celulares “Telenet”, en un rango de fechas 
dado (del 01/10/2020 al 31/10/2020).
*/
select tab.id, 
	tab.numero_telefono,
	llamada.tipo_llamada,
	llamada.operador,
	llamada.fecha,
	llamada.duracion,
	tab.id_sucursal as origen,
	llamada.id_destino as destino from
(select id, numero_telefono, contrato.id_sucursal from suscriptor
	inner join contrato
	on suscriptor.id = contrato.id_suscriptor) as tab
	inner join llamada
	on llamada.id_contrato = tab.numero_telefono and
		(llamada.operador = 'Telenet') and
		(llamada.fecha >= '2020/10/01' and llamada.fecha <= '2020/10/31') and
		(llamada.id_destino <> tab.id_sucursal);
