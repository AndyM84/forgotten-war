BEGIN TRY
	BEGIN TRANSACTION;

	IF EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserDevice')
	BEGIN
		DROP TABLE [dbo].[UserDevice]

		PRINT '''UserDevice'' was deleted'
	END

	COMMIT
END TRY
BEGIN CATCH
	PRINT 'There was an error in the script, rolling back'
	ROLLBACK

	SELECT ERROR_NUMBER() AS ErrorNumber, ERROR_LINE() AS ErrorLine, ERROR_MESSAGE() AS ErrorMessage
END CATCH