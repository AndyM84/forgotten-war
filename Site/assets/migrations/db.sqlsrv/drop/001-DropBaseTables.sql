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

BEGIN TRY
	BEGIN TRANSACTION;

	IF EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'LoginKey')
	BEGIN
		DROP TABLE [dbo].[LoginKey]

		PRINT '''LoginKey'' was deleted'
	END

	IF EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserRole')
	BEGIN
		DROP TABLE [dbo].[UserRole]

		PRINT '''UserRole'' was deleted'
	END

	IF EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserSession')
	BEGIN
		DROP TABLE [dbo].[UserSession]

		PRINT '''UserSession'' was deleted'
	END

	IF EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserAuthHistory')
	BEGIN
		DROP TABLE [dbo].[UserAuthHistory]

		PRINT '''UserAuthHistory'' was deleted'
	END

	IF EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserToken')
	BEGIN
		DROP TABLE [dbo].[UserToken]

		PRINT '''UserToken'' was deleted'
	END

	IF EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserProfile')
	BEGIN
		DROP TABLE [dbo].[UserProfile]

		PRINT '''UserProfile'' was deleted'
	END

	IF EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserSettings')
	BEGIN
		DROP TABLE [dbo].[UserSettings]

		PRINT '''UserSettings'' was deleted'
	END

	IF EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserVisibilities')
	BEGIN
		DROP TABLE [dbo].[UserVisibilities]

		PRINT '''UserVisibilities'' was deleted'
	END

	IF EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserContact')
	BEGIN
		DROP TABLE [dbo].[UserContact]

		PRINT '''UserContact'' was deleted'
	END	

	IF EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserRelationEvent')
	BEGIN
		DROP TABLE [dbo].[UserRelationEvent]

		PRINT '''UserRelationEvent'' was deleted'
	END

	IF EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserRelation')
	BEGIN
		DROP TABLE [dbo].[UserRelation]

		PRINT '''UserRelation'' was deleted'
	END

	IF EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'User')
	BEGIN
		DROP TABLE [dbo].[User]

		PRINT '''User'' was deleted'
	END

	IF EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'Role')
	BEGIN
		DROP TABLE [dbo].[Role]

		PRINT '''Role'' was deleted'
	END

	COMMIT
END TRY
BEGIN CATCH
	PRINT 'There was an error in the script, rolling back'
	ROLLBACK

	SELECT ERROR_NUMBER() AS ErrorNumber, ERROR_LINE() AS ErrorLine, ERROR_MESSAGE() AS ErrorMessage
END CATCH