# Libraries

We can use a library to enhance the features we can add to a project. In our 
use case, one of our most useful libraries is our CAN-Bus libary which allows
us to transmit messages to other devices in the car.

## Installing a Library

### Using the Arduino Library Manager

1. Open your Arduino IDE and navigate to the `Sketch` menu.

2. Proceed to `Sketch >> Include Library >> Manage Libraries...`

3. Pick a library you want to install

### Manually Installing a Library

1. Download the zip file for hte library you want to install (ex. from Github)

2. Extract the folder and place it in 
   `C:\Users\<User_Account>\Documents\Arduino\library`. Make sure the folder
   name does not include any special characters.

3. Restart the Arduino IDE

4. Navigate to `Sketch >> Include Library`, scroll down on the library list
   and you will see the library you just added.
   ([screenshot](manually-installed-library.png)) If nothing shows up make sure
   your `Sketchbook location` is `C:\Users\<User_Account>\Documents\Arduino` or
   the same location as the folder you just placed.