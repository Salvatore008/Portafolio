create table impuesto(
	abreviatura varchar(30) primary key,
	descripcion varchar(50),
	porcentaje real,
	fecha date
);

create table sucursal(
	codigo int,
	nombre varchar(30), --pais(internacional), localidad(nacional)
	abreviatura varchar(30),
	costo real,
	tipo_destino varchar(30), --llamada internacional o nacional
	fecha date,
	primary key (abreviatura)
);

create table cuesta(
	id_sucursal_origen varchar(30),
	codigo_origen int,
	id_sucursal_destino varchar(30),
	codigo_destino int,
	costo real,
	fecha date,
	primary key(id_sucursal_origen, id_sucursal_destino),
	foreign key (id_sucursal_origen)  references sucursal(abreviatura),
	foreign key (id_sucursal_destino) references sucursal(abreviatura)
);

create table suscriptor(id serial primary key not null);

create table persona_natural(
	id_suscriptor int primary key,
	cedula varchar(30),
	nombre varchar(30),
	apellido varchar(30),
	foreign key (id_suscriptor) references suscriptor(id)
);

create table persona_juridica(
	rif varchar(30) primary key,
	razon_social varchar(30),
	id_suscriptor int, 
	foreign key (id_suscriptor) references suscriptor(id)
);

create table tipo_renta(
	abreviatura varchar(30) primary key,
	descripcion varchar(30),
	costo real,
	cupo_impulsos_libres real,
	costo_impulsos_adicionales real,
	fecha date
);

create table contrato(
	numero_telefono varchar(30) primary key,
	id_suscriptor int,
	titular_pago int,
	id_tipo_renta varchar(30),
	tipo_pago varchar(30),
	tipo_telefono varchar(30),
	direccion_suministro varchar(50),
	direccion_notificacion varchar(50),
	id_sucursal varchar(30), --Sucursal que genera el numero de telefonov
	tarjeta_credito int, --En caso de tener, el pago es automatico
	foreign key (id_suscriptor) references suscriptor(id),
  	foreign key (titular_pago)  references suscriptor(id),
  	foreign key (id_tipo_renta) references tipo_renta(abreviatura),
  	foreign key (id_sucursal)   references sucursal(abreviatura)
);

create table pago(
	id serial primary key not null,
	tipo_pago varchar(30), --pago efectivo, tarjeta de debito o cheque conformable
	fecha date,
	costo real,
	id_contrato varchar(30),
	operador_telefonicio varchar(30),
	transaccion varchar(30), --numero de transaccion(tarjeta debito), numero de cheque(cheque)
	banco varchar(30), --banco emisor de la tarjeta o al que pertenece el cheque
	clave int, --ultimos 4 digitos de la tarjeta o clave del cheque
	foreign key (id_contrato) references contrato(numero_telefono)
);

create table compañia_celular(
	compañia varchar(30) primary key,
	costo_minuto real,
	fecha date
);

create table llamada(
	id serial primary key not null,
	id_contrato varchar(30),
	tipo_llamada varchar(30), --Internacional o nacional(celular, local y no local)
	numero_destino varchar(30),
	id_compañia_celular varchar(30),
	operador varchar(30),
	fecha date, --Fecha en la que se realizo la llamda
	duracion time,
	hora time, --Hora en que se realizo la llamada
	id_destino varchar(30),
	foreign key (id_contrato)         references contrato(numero_telefono),
	foreign key (id_compañia_celular) references compañia_celular(compañia),
	foreign key (id_destino)          references sucursal(abreviatura)
);

create table servicio(
	abreviatura varchar(30) primary key,
	descripcion varchar(30),
	costo real,
	fecha date
);

create table tiene_servicio(
	id_servicio varchar(30),
	id_contrato varchar(30),
	fecha_inicio date,
	fecha_final date,
	foreign key (id_servicio) references servicio(abreviatura),
	foreign key (id_contrato) references contrato(numero_telefono)
);
