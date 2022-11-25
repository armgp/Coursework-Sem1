# Software Systems Development
## Lab 13 – Flask Web framework & OOPs concept
### Date – 10th November 2022

	1. Configured SQLite as a database connection.
	2. An Endpoint to register user is created.
			i. Name – /user/signup
			ii. Method – post
			iii. Request –
				{
				 “name”: “Ashish”,
				 “email”: some@email.com,
				 “password”: “somepassword”
				}
				iv. Response –
				{
				 “message”: “Successfully Registered”
				}
	3. An Endpoint to login user is created.
			i. Name – /user/signin
			ii. Method – post
			iii. Request –
			{
			 “email”: “some@email.com”,
			 “password”: “somepassword”
			}
			iv. Response –
			{
			 “message”: “------”
			}

	4. An Endpoint to log out user.
			i. Name – /user/signout
			ii. Method – get
			iii. Response –
			{
			 “message”: “------”
			}
