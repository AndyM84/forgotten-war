BEGIN TRY
	BEGIN TRANSACTION;

		INSERT INTO [Role] ([Name], [Created]) VALUES ('Administrator', GETUTCDATE());

	COMMIT
END TRY
BEGIN CATCH
	PRINT 'There was an error in the script, rolling back'
	ROLLBACK

	SELECT ERROR_NUMBER() AS ErrorNumber, ERROR_LINE() AS ErrorLine, ERROR_MESSAGE() AS ErrorMessage
END CATCH