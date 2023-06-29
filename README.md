# Scanify - 3DScanner 
A 3D scanning approach that uses an IR Sharp sensor and stepper motors. By combining precise motor control and accurate distance measurements, the system enables the scanning of objects in three dimensions.

Through calibration and parameter adjustments, including scan resolution and stepper motor settings, the project achieves high accuracy in capturing the geometric data of scanned objects.

The process involves iteratively scanning the object at various Z-axis heights, resulting in a comprehensive 3D model. With its geometric calculations and control mechanisms, this project contributes to the development of an accurate 3D object reconstruction.

## Geometric Calculations to get the XYZ Coordinates

### Distance Calculation:
To begin, we calculate the distance between the sensor and the surface of the object being scanned. This value is crucial for accurately determining the object's position. Mathematically, we express it as the difference between the Distance To Center and the Distance To Object Surface:


<p align="center">
<b>Distance = Distance to Center - Distance to Object Surface<b>
</p>

<p align="center">
<img width="351" height="430" src="https://github.com/H3EsAwY/Scanify-3DScanner/assets/75787889/c73e140b-8210-40fb-a9ea-778a7d75d4dd">
</p>


### XY Coordinate Calculation
The XY coordinates of the object are derived from the calculated distance and the known angular displacement. By using trigonometric functions, we determine the displacement along the X and Y axes relative to the origin, which is the center of the table. Specifically, we utilize the sine and cosine functions to calculate the X and Y coordinates, respectively. The equations for calculating the  XY coordinates are as follows:

<p align="center">
<img src="https://github.com/H3EsAwY/Scanify-3DScanner/assets/75787889/f59c1baa-ab95-4ce5-91c8-a130583d0cc0">
</p>

<p align="center">
<img width="380" height="380" src="https://github.com/H3EsAwY/Scanify-3DScanner/assets/75787889/83c5cb57-8dbe-4168-b222-610175ac7148">
</p>

<p align="center">
<b>X = Distance * sin(angle)<b>
</p>
<p align="center">
<b>Y = Distance * cos(angle)<b>
</p>

Here, the angle represents the angular displacement, which is obtained by incrementing the `Angle` by the angle alpha moved in each step.
<p align="center">
<b>Angle = Angle + Alpha<b>
</p>
We can get the angle alpha by dividing the total angle for one revolution (360 degrees) by the number of steps in one revolution (200 steps). Consequently, each step corresponds to an angle of 1.8 degrees.

### Z Coordinate Calculation
To calculate the Z-coordinate of a scanned object in the 3D scanning process, we can employ the concept of step height and the number of steps taken during the scanning process.
The step height refers to the vertical distance the sensor moves up after completing one full revolution at a particular Z-axis height. By multiplying the step height with the number of steps taken during the scanning process, we can determine the total vertical displacement of the sensor.

**For example**, if the step height is set to 1 cm and the scanning process involves 100 steps, the total vertical displacement would be 100 cm.

To calculate the Z-coordinate, we divide the total vertical displacement by the number of steps in one revolution. This gives us the Z-coordinate value for each step taken during the scanning process.
<p align="center">
<b>z = z + z_layer_height<b>
</p>
where the `z_layer_height` is the distance moved vertically after each complete turn table rotation.

**So by incrementally increasing the Z-coordinate value during the scanning process**, we can capture the object's profile at different vertical levels, ultimately generating a comprehensive 3D representation of the scanned object.

