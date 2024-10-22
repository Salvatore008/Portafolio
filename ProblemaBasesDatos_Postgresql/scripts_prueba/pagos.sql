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


select * from pago;

