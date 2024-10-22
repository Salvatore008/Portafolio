-- Funciones
create or replace function new_persona_natural(
				cedula varchar(30),
				nombre varchar(30),
				apellido varchar(30)
) 
returns void as
$$
declare
	id_suscriptor int;
begin
	insert into suscriptor values (default);
	
	select id into id_suscriptor from suscriptor
	order by id desc limit 1;
	
	insert into persona_natural 
	values ( id_suscriptor,
	         cedula,
			 nombre,
			 apellido
			 );
end;
$$
language plpgsql;

create or replace function new_persona_juridica(
				rif varchar(30),
				razon_social varchar(30)
) 
returns void as
$$
declare
	id_suscriptor int;
begin
	insert into suscriptor values (default);
	
	select id into id_suscriptor from suscriptor
	order by id desc limit 1;
	
	insert into persona_juridica
	values (rif,
			razon_social,
			id_suscriptor
			 );
end;
$$
language plpgsql;

create or replace function new_sucursal(
				codigo int,
				nombre varchar(30),
				abreviatura varchar(30),
				tipo_destino varchar(30),
				fecha date default null,
				costo real default null
) 
returns void as
$$
begin
	
	insert into sucursal 
	values (codigo,
	        nombre,
			abreviatura,
			costo,
			tipo_destino,
			fecha
			 );
end;
$$
language plpgsql;

create or replace function new_cuesta(
				origen varchar(30),
				codigo_origen int,
				destino varchar(30),
				codigo_destino int,
				costo real,
				fecha date
) 
returns void as
$$
begin
	insert into cuesta
	values (origen,
			codigo_origen,
			destino,
			codigo_destino,
			costo,
			fecha
			 );
end;
$$
language plpgsql;

create or replace function new_impuesto(
				abreviatura varchar(30),
				descripcion varchar(50),
				porcentaje real,
				fecha date
) 
returns void as
$$
begin
	insert into impuesto
	values (abreviatura,
			descripcion,
			porcentaje,
			fecha
			 );
end;
$$
language plpgsql;

create or replace function new_compañia_celular(
				compañia varchar(30),
				costo_minuto real,
				fecha date
) 
returns void as
$$
begin
	insert into compañia_celular
	values (compañia,
			costo_minuto,
			fecha
			 );
end;
$$
language plpgsql;

create or replace function new_servicio(
				abreviatura varchar(30),
				descripcion varchar(30),
				costo real
) 
returns void as
$$
begin
	insert into servicio
	values (abreviatura,
			descripcion,
			costo
			 );
end;
$$
language plpgsql;

create or replace function new_tipo_renta(
				abreviatura varchar(30),
				descripcion varchar(30),
				costo real,
				cupo_impulsos_libres real,
				costo_impulsos_adicionales real,
				fecha date default null
) 
returns void as
$$
begin
	insert into tipo_renta
	values (abreviatura,
			descripcion,
			costo,
			cupo_impulsos_libres,
			costo_impulsos_adicionales,
			fecha
			 );
end;
$$
language plpgsql;


create or replace function new_contrato(
				telefono varchar(30),
				suscriptor int,
				titular_pago int,
				tipo_renta varchar(30),
				tipo_pago varchar(30),
				tipo_telefono varchar(30),
				direccion_suministro varchar(30),
				direccion_notificacion varchar(30),
				id_sucursal varchar(30) default null,
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

create or replace function new_pago(
				tipo_pago varchar(30),
				fecha date,
				costo real,
				id_contrato varchar(30),
				clave int default null, --clave de cheque o ultimos 4 digitos de la tarjeta de debito.
				transaccion varchar(30) default null, --Numero e transaccion o numero de cheque
				operador_telefonico varchar(30) default null,
				banco varchar(30) default null
) 
returns void as
$$
begin
	insert into pago
	values (default,
			tipo_pago,
			fecha,
			costo,
			id_contrato,
			operador_telefonico,	
			transaccion,
			banco,
			clave
			 );
end;
$$
language plpgsql;

create or replace function new_llamada(
				id_contrato varchar(30),
				tipo_llamada varchar(30), --Internacional o nacional(celular, local y no local)
				operador varchar(30),
				fecha date,
				duracion time,
				hora time,
				id_destino varchar(30) default null,
				numero_destino varchar(30) default null,
				id_compañia_celular varchar(30) default null
) 
returns void as
$$
begin
	insert into llamada
	values (default,
			id_contrato,
			tipo_llamada,
			numero_destino,
			id_compañia_celular,
			operador,
			fecha,
			duracion,
			hora,
			id_destino
			 );
end;
$$
language plpgsql;

create or replace function new_tiene_servicio(
				id_servicio varchar(30),
				id_contrato varchar(30),
				fecha_inicio date,
				fecha_final date
) 
returns void as
$$
begin
	insert into tiene_servicio
	values (id_servicio,
			id_contrato,
			fecha_inicio,
			fecha_final
			 );
end
$$
language plpgsql;

--Datos de personas
select new_persona_natural(
	        'V-4.890.639',
			'José Eladio',
			 'Martínez Díaz'
			 );

select new_persona_natural(
	        'V-6.345.012',
			'Milex',
			 'Deris Nieto'
			 );

select new_persona_natural(
	        'V-3.855.721',
			'Vicenta',
			 'Rivero de Bracho'
			 );

select new_persona_juridica(
	        'J-02690345-7',
			'Sistemas J.J y Asociados'
			 );

--Datos de sucursales
select new_sucursal(0212,
					'Caracas',
					'CAR',
					'nacional'
					);

select new_sucursal(0232,
					'Los Teques',
					'LTQ',
					'nacional'
					);

select new_sucursal(0233,
					'Carayaca',
					'CAZ',
					'nacional'
					);

select new_sucursal(0234,
					'Los Velásquez',
					'AVE',
					'nacional'
					);

select new_sucursal(034,
					'España',
					'ESP',
					'internacional',
					'2020/08/01',
					9.80
					);

select new_sucursal(039,
					'Italia',
					'ITA',
					'internacional',
					'2020/08/01',
					10.00
					);

select new_sucursal(001,
					'Estados Unidos de América',
					'EUA',
					'internacional',
					'2020/08/01',
					7.60
					);

select new_sucursal(000,
					'Movilcel',
					'MOV',
					'nacional',
					'2020/07/01',
					14.60
					);

select new_sucursal(000,
					'Telenet',
					'TEL',
					'nacional',
					'2020/09/01',
					15.00
					);

--Datos de servicios
select new_servicio('STA',
					'Servicio de Teleamigo',
					200.0
					);

select new_servicio('SNP',
					'Servicio de Número Privado',
					400.0
					);

--Datos de tipo de renta
select new_tipo_renta('BAS',
					'Básica',
					194,
					50,
					5.17
					);

select new_tipo_renta('INT',
					'Intermedia',
					304,
					90,
					3.69
					);

select new_tipo_renta('REN',
					'Rendidora',
					555,
					130,
					3.19
					);

select new_tipo_renta('UNI',
					'Única',
					800,
					0,
					5.75
					);

--Datos de impuestos
select new_impuesto('ICS',
					'Impuesto al Consumo Suntuario',
					0.16,
					'2020/09/01'
					);

select new_impuesto('UCI',
					'Impuesto al Uso de la Conexión Internacional',
					0.25,
					'2020/09/01'
					);

select new_impuesto('IMR',
					'Impuesto al Mantenimiento de la Red',
					0.01,
					'2020/09/01'
					);

select new_impuesto('IVM',
					'Impuesto de Ventas al Mayor',
					0.125,
					'2020/09/01'
					);

--Datos de cuesta(relacion entre sucursal y sucursal)
select new_cuesta('AVE',
					0234,
					'CAR',
					0212,
					5.80,
					'2020/08/01'
					);

select new_cuesta('AVE',
					0234,
					'LTQ',
					0232,
					3.00,
					'2020/08/01'
					);

select new_cuesta('AVE',
					0234,
					'CAZ',
					0233,
					2.70,
					'2020/08/01'
					);

select new_cuesta('CAR',
					0212,
					'LTQ',
					0232,
					4.50,
					'2020/08/01'
					);

select new_cuesta('CAR',
					0212,
					'CAZ',
					0233,
					3.60,
					'2020/08/01'
					);

select new_cuesta('CAZ',
					0233,
					'LTQ',
					0232,
					2.50,
					'2020/08/01'
					);

--Datos de compañia celular
select new_compañia_celular('MOV',
							14.6,
							'2020/07/01'
							);

select new_compañia_celular('TEL',
							15.0,
							'2020/09/01'
							);

--Datos de contratos
select new_contrato('0234-2576389',
					1,
					1,
					'BAS',
					'efectivo',	
					'Residencial',
					'Urb. Carrizales, calle 14, Nro. 153',
					'Urb. Carrizales, calle 14, Nro. 153',
					'AVE'
					);

select new_contrato('0212-6622590',
					2,
					2,
					'INT',
					'efectivo-cheque',	
					'Residencial',
					'Urb. La Colina, Edif. Girasol, Apto. 22',
					'Urb. La Colina, Edif. Girasol, Apto. 22',
					'CAR'
					);

select new_contrato('0212-4844356',
					3,
					3,
					'REN',
					'efectivo',	
					'Residencial',
					'Urb. La Cascada, Edif. Némesis, Apto. 5-A',
					'Urb. La Cascada, Edif. Némesis, Apto. 5-A',
					'CAR'
					);

select new_contrato('0212-4435560',
					4,
					4,
					'UNI',
					'tarjeta debito',	
					'Comercial',
					'C.C. Centro Plaza, nivel 3, local 35',
					'C.C. Centro Plaza, nivel 3, local 35',
					'CAR'
					);

--Datos de pago
select new_pago('efectivo',
				'2020/10/04',
				576.80,
				'0234-2576389'
				);

select new_pago('efectivo',
				'2020/10/27',
				30,
				'0212-6622590'
				);

select new_pago('cheque',
				'2020/10/28',
				257.50,
				'0212-6622590',
				3443,
				'3445645646',
				'43',
				'Banco de Venezuela'
				);

select new_pago('efectivo',
				'2020/10/01',
				2400,
				'0212-4844356'
				);

select new_pago('tarjeta debito',
				'2020/10/27',
				3747.85,
				'0212-4435560',
				4554,
				'333333333333'
				);

--Datos de llamadas
select new_llamada('0234-2576389',
					'nacional',
					'DDN',
					'2020/10/05',
					'00:00:41',
					'21:35:09',
					'CAR',
					'0212-9063999'
					);

select new_llamada('0234-2576389',
					'nacional',
					'DDN',
					'15/10/2020',
					'00:01:02',
					'12:42:09',
					'LTQ',
					'0232-2344441'
					);

select new_llamada('0234-2576389',
					'nacional',
					'DDN',
					'26/10/2020',
					'00:03:39',
					'08:06:29',
					'CAZ',
					'0233-345551'
					);

select new_llamada('0234-2576389',
					'internacional',
					'operadora',
					'17/10/2020',
					'00:01:16',
					'11:28:39',
					'CAR',
					'0212-9773044'
					);

select new_llamada('0234-2576389',
					'internacional',
					'DDI',
					'20/10/2020',
					'00:06:00',
					'06:20:06',
					'ESP',
					'0340034439007'
					);

select new_llamada('0234-2576389',
					'internacional',
					'DDI',
					'24/10/2020',
					'00:02:00',
					'22:07:00',
					'EUA',
					'0019082231083'
					);

select new_llamada('0212-6622590',
					'nacional',
					'Movilcel',
					'30/10/2020',
					'00:09:38',
					'20:06:29',
					'CAR',
					'0416-7345559',
					'MOV'
					);

select new_llamada('0212-6622590',
					'nacional',
					'Movilcel',
					'30/10/2020',
					'00:10:00',
					'20:06:29',
					'MOV'
					);

select new_llamada('0212-6622590',
					'nacional',
					'Telenet',
					'20/10/2020',
					'00:06:43',
					'16:20:01',
					'CAR',
					'0414-0319889',
					'TEL'
					);

select new_llamada('0212-6622590',
					'nacional',
					'Telenet',
					'20/10/2020',
					'00:07:00',
					'16:20:01',
					'TEL'
					);

select new_llamada('0212-6622590',
					'nacional',
					'DDN',
					'20/10/2020',
					'00:01:42',
					'12:42:09',
					'LTQ',
					'0232-6334942'
					);

select new_llamada('0212-4844356',
					'nacional',
					'Movilcel',
					'20/10/2020',
					'00:09:38',
					'20:06:29',
					'CAR',
					'0416-5312345'
					);

select new_llamada('0212-4844356',
					'nacional',
					'Movilcel',
					'20/10/2020',
					'00:10:00',
					'20:06:29',
					'MOV'
					);

select new_llamada('0212-4844356',
					'nacional',
					'Movilcel',
					'20/10/2020',
					'00:11:38',
					'20:20:29',
					'CAR',
					'0416-5312345'
					);

select new_llamada('0212-4844356',
					'nacional',
					'Movilcel',
					'20/10/2020',
					'00:12:00',
					'20:20:29',
					'MOV'
					);

select new_llamada('0212-4844356',
					'nacional',
					'Telenet',
					'20/10/2020',
					'00:01:08',
					'20:17:30',
					'CAR',
					'0414-2837200'
					);

select new_llamada('0212-4844356',
					'nacional',
					'Telenet',
					'20/10/2020',
					'00:02:00',
					'20:17:30',
					'TEL'
					);


select new_llamada('0212-4844356',
					'nacional',
					'DDN',
					'16/10/2020',
					'00:11:23',
					'12:42:09',
					'LTQ',
					'0232-6334942'
					);

select new_llamada('0212-4844356',
					'nacional',
					'DDN',
					'17/10/2020',
					'00:31:23',
					'02:40:01',
					'LTQ',
					'0232-6334942'
					);

select new_llamada('0212-4844356',
					'nacional',
					'DDN',
					'17/10/2020',
					'01:30:23',
					'14:00:06',
					'LTQ',
					'0232-6334942'
					);

select new_llamada('0212-4844356',
					'internacional',
					'DDN',
					'20/10/2020',
					'00:26:00',
					'16:20:06',
					'ESP',
					'0340014490207'
					);

select new_llamada('0212-4844356',
					'internacional',
					'DDN',
					'22/10/2020',
					'00:47:00',
					'06:00:36',
					'ESP',
					'0340014490207'
					);

select new_llamada('0212-4844356',
					'internacional',
					'DDN',
					'22/10/2020',
					'00:07:00',
					'22:00:36',
					'ITA',
					'0390312407563'
					);

select new_llamada('0212-4844356',
					'internacional',
					'operadora',
					'17/10/2020',
					'00:21:00',
					'00:00:54',
					'EUA',
					'0019031247563'
					);

select new_llamada('0212-4435560',
					'nacional',
					'Movilcel',
					'12/10/2020',
					'00:19:38',
					'20:06:29',
					'CAR',
					'0416-5312345',
					'MOV'
					);

select new_llamada('0212-4435560',
					'nacional',
					'Movilcel',
					'12/10/2020',
					'00:20:00',
					'20:06:29',
					'MOV'
					);

select new_llamada('0212-4435560',
					'nacional',
					'Movilcel',
					'14/10/2020',
					'00:01:38',
					'20:40:29',
					'CAR',
					'0416-8312347',
					'MOV'
					);

select new_llamada('0212-4435560',
					'nacional',
					'Movilcel',
					'14/10/2020',
					'00:02:00',
					'20:40:29',
					'MOV'
					);

select new_llamada('0212-4435560',
					'nacional',
					'Telenet',
					'18/10/2020',
					'00:11:08',
					'20:17:30',
					'CAR',
					'0414-2837201'
					);

select new_llamada('0212-4435560',
					'nacional',
					'Telenet',
					'18/10/2020',
					'00:12:00',
					'20:17:30',
					'TEL'
					);

select new_llamada('0212-4435560',
					'nacional',
					'Telenet',
					'19/10/2020',
					'00:13:08',
					'10:17:30',
					'LTQ',
					'0414-2833402'
					);

select new_llamada('0212-4435560',
					'nacional',
					'Telenet',
					'19/10/2020',
					'00:14:00',
					'10:17:30',
					'TEL'
					);

select new_llamada('0212-4435560',
					'internacional',
					'DDI',
					'15/10/2020',
					'00:12:00',
					'12:00:36',
					'ITA',
					'0390312199098'
					);

select new_llamada('0212-4435560',
					'internacional',
					'DDI',
					'16/10/2020',
					'00:12:00',
					'12:00:32',
					'ITA',
					'0390312199098'
					);

select new_llamada('0212-4435560',
					'internacional',
					'DDI',
					'17/10/2020',
					'00:12:00',
					'21:00:33',
					'ITA',
					'0390312199098'
					);

select new_llamada('0212-4435560',
					'internacional',
					'DDI',
					'18/10/2020',
					'00:12:00',
					'13:00:55',
					'ITA',
					'0390312199098'
					);

select new_llamada('0212-4435560',
					'internacional',
					'DDI',
					'19/10/2020',
					'00:12:00',
					'14:00:13',
					'ITA',
					'0390312199098'
					);

select new_llamada('0212-4435560',
					'internacional',
					'DDI',
					'20/10/2020',
					'00:12:00',
					'22:00:24',
					'ITA',
					'0390312199098'
					);

select new_llamada('0212-4435560',
					'internacional',
					'DDI',
					'21/10/2020',
					'00:16:00',
					'16:20:06',
					'ESP',
					'0340014480072'
					);

select new_llamada('0212-4435560',
					'internacional',
					'DDI',
					'22/10/2020',
					'00:17:00',
					'06:00:36',
					'ESP',
					'0340014341992'
					);

select new_llamada('0212-4435560',
					'internacional',
					'DDI',
					'22/10/2020',
					'00:12:00',
					'22:00:36',
					'ITA',
					'0390312199098'
					);

select new_llamada('0212-4435560',
					'internacional',
					'operadora',
					'17/10/2020',
					'00:21:00',
					'00:00:54',
					'EUA',
					'0019031247563'
					);

select new_llamada('0212-4435560',
					'internacional',
					'operadora',
					'18/10/2020',
					'00:51:00',
					'10:00:43',
					'EUA',
					'0019031247563'
					);

select new_llamada('0212-4435560',
					'internacional',
					'operadora',
					'18/10/2020',
					'00:11:00',
					'11:14:33',
					'EUA',
					'0019031247563'
					);

select new_llamada('0212-4435560',
					'internacional',
					'operadora',
					'19/10/2020',
					'00:01:00',
					'11:00:41',
					'EUA',
					'0019031247563'
					);

--Datos de tiene_servicios
select new_tiene_servicio('STA',
						'0212-6622590',
						'2020/10/01',
						'2020/10/25'
						);

select new_tiene_servicio('SNP',
						'0212-6622590',
						'2020/10/01',
						'2020/10/25'
						);

select new_tiene_servicio('STA',
						'0212-4844356',
						'2020/10/19',
						'2020/11/13'
						);