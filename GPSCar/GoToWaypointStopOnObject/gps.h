#ifndef BLUETOOTH_H

	#define BLUETOOTH_H
	unsigned int MAX_BUFFER_LENGTH = 100;
 
	void Step_ValidateWaypoint( void );
  bool ValidateSentence( void );
  bool ReadAllStream( void );
  bool ValidateData( void ); 
#endif
