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


