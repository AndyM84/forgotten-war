/*

	Attempts to create the following tables:
		- dbo.User: Base account data for individual users
		- dbo.LoginKey: Login keys, 1+ for all users that can login
		- dbo.Role: Roles defined in system
		- dbo.UserRole: Entries that show which roles to which a user belongs
		- dbo.UserSession: User session storage for access control
		- dbo.UserAuthHistory: Historical auth-related actions for users (doesn't include data, merely audit trail)
		- dbo.UserToken: User token storage (for emails/etc) w/ text field for data context
		- dbo.UserProfile: Profile information for users
		- dbo.UserSettings: Settings data for users
		- dbo.UserVisibilities: Basic visibility settings for user information
		- dbo.UserContact: Collection of contact data for users
		- dbo.UserRelation: Table with mirror values for user relationships (friend -> family -> bestie? -> dating?), mirrored stage == accepted stage ('invite' means stages !=)
		- dbo.UserRelationEvent: Table to track actions related to user relationship changes

*/

DROP TABLE `LoginKey`;
DROP TABLE `UserRole`;
DROP TABLE `UserSession`;
DROP TABLE `UserAuthHistory`;
DROP TABLE `UserToken`;
DROP TABLE `UserProfile`;
DROP TABLE `UserSettings`;
DROP TABLE `UserVisibilities`;
DROP TABLE `UserContact`;
DROP TABLE `UserRelationEvent`;
DROP TABLE `UserRelation`;
DROP TABLE `User`;
DROP TABLE `Role`;