# Adafruit Unified Sensor Driver #

Many small embedded systems exist to collect data from sensors, analyse the data, and either take an appropriate action or send that sensor data to another system for processing.

One of the many challenges of embedded systems design is the fact that parts you used today may be out of production tomorrow, or system requirements may change and you may need to choose a different sensor down the road.

Creating new drivers is a relatively easy task, but integrating them into existing systems is both error prone and time consuming since sensors rarely use the exact same units of measurement.

By reducing all data to a single **sensors\_event\_t** 'type' and settling on specific, **standardised SI units** for each sensor family the same sensor types return values that are comparable with any other similar sensor.  This enables you to switch sensor models with very little impact on the rest of the system, which can help mitigate some of the risks and problems of sensor availability and code reuse.

The unified sensor abstraction layer is also useful for data-logging and data-transmission since you only have one well-known type to log or transmit over the air or wire.

## Unified Sensor Drivers ##

The following drivers are based on the Adafruit Unified Sensor Driver:

**Accelerometers**
  - [Adafruit\_ADXL345](https://github.com/adafruit/Adafruit_ADXL345)
  - [Adafruit\_LSM303DLHC](https://github.com/adafruit/Adafruit_LSM303DLHC)
  - [Adafruit\_MMA8451\_Library](https://github.com/adafruit/Adafruit_MMA8451_Library)

**Gyroscope**
  - [Adafruit\_L3GD20\_U](https://github.com/adafruit/Adafruit_L3GD20_U)

**Light**
  - [Adafruit\_TSL2561](https://github.com/adafruit/Adafruit_TSL2561)
  - [Adafruit\_TSL2591\_Library](https://github.com/adafruit/Adafruit_TSL2591_Library)

**Magnetometers**
  - [Adafruit\_LSM303DLHC](https://github.com/adafruit/Adafruit_LSM303DLHC)
  - [Adafruit\_HMC5883\_Unified](https://github.com/adafruit/Adafruit_HMC5883_Unified)
  
**Barometric Pressure**
  - [Adafruit\_BMP085\_Unified](https://github.com/adafruit/Adafruit_BMP085_Unified)
  - [Adafruit\_BMP183\_Unified\_Library](https://github.com/adafruit/Adafruit_BMP183_Unified_Library)

**Humidity & Temperature**
  - [Adafruit\_DHT\_Unified](https://github.com/adafruit/Adafruit_DHT_Unified)

## How Does it Work? ##

Any driver that supports the Adafruit unified sensor abstraction layer will implement the Adafruit\_Sensor base class.  There are two main typedefs and one enum defined in Adafruit_Sensor.h that are used to 'abstract' away the sensor details and values:

**Sensor Types (sensors\_type\_t)**

These pre-defined sensor types are used to properly handle the two related typedefs below, and allows us determine what types of units the sensor uses, etc.

```
/** Sensor types */
typedef enum
{
  SENSOR_TYPE_ACCELEROMETER         = (1),
  SENSOR_TYPE_MAGNETIC_FIELD        = (2),
  SENSOR_TYPE_ORIENTATION           = (3),
  SENSOR_TYPE_GYROSCOPE             = (4),
  SENSOR_TYPE_LIGHT                 = (5),
  SENSOR_TYPE_PRESSURE              = (6),
  SENSOR_TYPE_PROXIMITY             = (8),
  SENSOR_TYPE_GRAVITY               = (9),
  SENSOR_TYPE_LINEAR_ACCELERATION   = (10),
  SENSOR_TYPE_ROTATION_VECTOR       = (11),
  SENSOR_TYPE_RELATIVE_HUMIDITY     = (12),
  SENSOR_TYPE_AMBIENT_TEMPERATURE   = (13),
  SENSOR_TYPE_VOLTAGE               = (15),
  SENSOR_TYPE_CURRENT               = (16),
  SENSOR_TYPE_COLOR                 = (17)
} sensors_type_t;
```

**Sensor Details (sensor\_t)**

This typedef describes the specific capabilities of this sensor, and allows us to know what sensor we are using beneath the abstraction layer.

```
/* Sensor details (40 bytes) */
/** struct sensor_s is used to describe basic information about a specific sensor. */
typedef struct
{
    char     name[12];
    int32_t  version;
    int32_t  sensor_id;
    int32_t  type;
    float    max_value;
    float    min_value;
    float    resolution;
    int32_t  min_delay;
} sensor_t;
```

The individual fields are intended to be used as follows:

- **name**: The sensor name or ID, up to a maximum of twelve characters (ex. "MPL115A2")
- **version**: The version of the sensor HW and the driver to allow us to differentiate versions of the board or driver
- **sensor\_id**: A unique sensor identifier that is used to differentiate this specific sensor instance from any others that are present on the system or in the sensor network
- **type**: The sensor type, based on **sensors\_type\_t** in sensors.h
- **max\_value**: The maximum value that this sensor can return (in the appropriate SI unit)
- **min\_value**: The minimum value that this sensor can return (in the appropriate SI unit)
- **resolution**: The smallest difference between two values that this sensor can report (in the appropriate SI unit)
- **min\_delay**: The minimum delay in microseconds between two sensor events, or '0' if there is no constant sensor rate

**Sensor Data/Events (sensors\_event\_t)**

This typedef is used to return sensor data from any sensor supported by the abstraction layer, using standard SI units and scales.

```
/* Sensor event (36 bytes) */
/** struct sensor_event_s is used to provide a single sensor event in a common format. */
typedef struct
{
    int32_t version;
    int32_t sensor_id;
    int32_t type;
    int32_t reserved0;
    int32_t timestamp;
    union
    {
        float           data[4];
        sensors_vec_t   acceleration;
        sensors_vec_t   magnetic;
        sensors_vec_t   orientation;
        sensors_vec_t   gyro;
        float           temperature;
        float           distance;
        float           light;
        float           pressure;
        float           relative_humidity;
        float           current;
        float           voltage;
        sensors_color_t color;
    };
} sensors_event_t;
```
It includes the following fields:

- **version**: Contain 'sizeof(sensors\_event\_t)' to identify which version of the API we're using in case this changes in the future
- **sensor\_id**: A unique sensor identifier that is used to differentiate this specific sensor instance from any others that are present on the system or in the sensor network (must match the sensor\_id value in the corresponding sensor\_t enum above!)
- **type**: the sensor type, based on **sensors\_type\_t** in sensors.h
- **timestamp**: time in milliseconds when the sensor value was read
- **data[4]**: An array of four 32-bit values that allows us to encapsulate any type of sensor data via a simple union (further described below)

**Required Functions**

In addition to the two s@
  r@ d9`@! L$ 4A ra
R tIp  D*(    d"iR 0 baqE@ Gh AD`buHdB@d@E@ )$ ! P  )0 a@a@ h "I, n7)  & b` @@#L2

   @Ja $EdA@Ah @$&@ $@ P2    @ ,	G dHAr $@hb Inb ! , `PD !t` $h! Sq@l	d` @ b E2 %E, T\$ @`F%Reja Ghp  t a $D Ar ,!@ D b!jr"` $`p!$  P,Q s`kE@d "(, @@@0 "A,A`)
& a  ` f !r q* S`"p   D 4D pkp !d`  @ D@  $d@d*Q 0aFPo2Ip
 !
 ` 
``,	." 4` " "1"T	 4h `O4 $e 2Id "` 	C $.$ pH (  b 0 D( @B$  $ ! @1o" "a(A, "a@ @"ag$ -& A @@ `Dq`C$ `0b**(RT ` @pD   @  & h!$! M2 1e NPX  2 D* 
@ `  @r O" P@a a`A P  !	   A`R (1 t$ `,DBda"!T	h N 0`L e# i
 R@  B@  M@ A PaR (bE!2 S D( 1H`#` 	 @`P Hp`a   @` P  $!t 0X ,a/* & aDc@P2\ @dd4X `"   P`B " d P"hc
 HFha ec  @!H @ !"  @@@J #e$ " 0 ,  J "$r (% B
l@	)hd 	 @`DQ AD 1!@$c
 ,  (aAblb 0!GJ"   B@heap `A `h  "  tA   a  3D!N@@ 0d2 PDa&N 
 	( !  
dLE ( "AD0E"  2d ` "a C0faLA
   T	  
*kp	a"`aL  ( R @a B a`a h@ 
  e%#*   `x2D"(0 $ h1$c  r  (J "* ad* ) (p!HpD0 T@%""  @dpE `L **ddb2@!a aDJ@I$@$(  $(a@QB	 ( ($hC !`e" 
 RALTA   !D 
#!Dti!dp@ $ 
(iB(@*" VA t`# rd AF  Q	 8 @	d0
 (( @!sQ @
  v@e  ! h H%Cl  @L
* (@@! ) *`$   !$DPi`P	
 ` P  a"A aN *" %PCeF`*(  *(a@R`&  0 P!ha@s  0  & 
 @Hhia%pS " ( @ )  *dLG  R`,0@a !PE 	$ 
bnlPr 
 (	 , ( ch@
B * ( @   PA 	j 0,    RB@ c A$    (ti`FeIt@ D   	"i` @@ oPD!4
 #  P`$ Qd `dD @ B" @`"0b@@ aJ @qer in Practice ##

Using the unified sensor abstraction layer is relatively easy once a compliant driver has been created.

Every compliant sensor can now be read using a single, well-known 'type' (sensors\_event\_t), and there is a standardised way of interrogating a sensor about its specific capabilities (via sensor\_t).

An example of reading the [TSL2561](https://github.com/adafruit/Adafruit_TSL2561) light sensor can be seen below:

```
 Adafruit_TSL2561 tsl = Adafruit_TSL2561(TSL2561_ADDR_FLOAT, 12345);
 ...
 /* Get a new sensor event */ 
 sensors_event_t event;
 tsl.getEvent(&event);
 
 /* Display the results (light is measured in lux) */
 if (event.light)
 {
   Serial.print(event.light); Serial.println(" lux");
 }
 else
 {
   /* If event.light = 0 lux the sensor is probably saturated
      and no reliable data could be generated! */
   Serial.println("Sensor overload");
 }
```

Similarly, we can get the basic technical capabilities of this sensor with the following code:

```
 sensor_t sensor;
 
 sensor_t sensor;
 tsl.getSensor(&sensor);

 /* Display the sensor details */
 Serial.println("------------------------------------");
 Serial.print  ("Sensor:       "); Serial.println(sensor.name);
 Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
 Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
 Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
 Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
 Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
 Serial.println("------------------------------------");
 Serial.println("");
```
