create or replace function new_contrato(
				telefono varchar(30),
				suscriptor int,
				titular_pago int,
				tipo_renta varchar(30),
				tipo_pago varchar(30),
				tipo_telefono varchar(30),
				direccion_suministro varchar(30),
				direccion_notificacion varchar(30),
				id_sucursal int default null,
				tarjeta_credito int default null
) 
returns void as
$$
begin
	insert into contrato
	values (telefono,
			suscriptor,
			titular_pago,
			tipo_renta,
			tipo_pago,
			tipo_telefono,
			direccion_suministro,
			direccion_notificacion,
			id_sucursal,
			tarjeta_credito
			 );
end;
$$
language plpgsql;

select new_contrato('0234-2576389',
					1,
					1,
					'BAS',
					'efectivo',	
					'Residencial',
					'Urb. Carrizales, calle 14, Nro. 153',
					'Urb. Carrizales, calle 14, Nro. 153'
					);

select new_contrato('0212-6622590',
					2,
					2,
					'INT',
					'efectivo-cheque',	
					'Residencial',
					'Urb. La Colina, Edif. Girasol, Apto. 22',
					'Urb. La Colina, Edif. Girasol, Apto. 22'
					);

select new_contrato('0212-4844356',
					3,
					3,
					'REN',
					'efectivo',	
					'Residencial',
					'Urb. La Cascada, Edif. Némesis, Apto. 5-A',
					'Urb. La Cascada, Edif. Némesis, Apto. 5-A'
					);

select new_contrato('0212-4435560',
					4,
					4,
					'UNI',
					'tarjeta debito',	
					'Comercial',
					'C.C. Centro Plaza, nivel 3, local 35',
					'C.C. Centro Plaza, nivel 3, local 35'
					);
