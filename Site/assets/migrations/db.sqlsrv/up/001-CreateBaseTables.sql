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

	/* User Table */
	IF NOT EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'User')
	BEGIN
		SET ANSI_NULLS ON;
		SET QUOTED_IDENTIFIER ON;

		CREATE TABLE [dbo].[User] (
			[ID] INT IDENTITY(1,1) NOT NULL,
			[Email] NVARCHAR(256) NOT NULL,
			[EmailConfirmed] BIT NOT NULL,
			[Joined] DATETIME2(7) NOT NULL,
			[LastLogin] DATETIME2(7) NULL,
			[LastActive] DATETIME2(7) NULL,
			CONSTRAINT [PK_dbo.User] PRIMARY KEY CLUSTERED (
				[ID] ASC
			) WITH (
				PAD_INDEX = OFF,
				STATISTICS_NORECOMPUTE = OFF,
				IGNORE_DUP_KEY = OFF,
				ALLOW_ROW_LOCKS = ON,
				ALLOW_PAGE_LOCKS = ON
			) ON [PRIMARY]
		) ON [PRIMARY];

		PRINT '''User'' table was created'
	END
	ELSE
		PRINT '''User'' table already exists'

	/* LoginKey Table */
	IF NOT EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'LoginKey')
	BEGIN
		SET ANSI_NULLS ON;
		SET QUOTED_IDENTIFIER ON;

		CREATE TABLE [dbo].[LoginKey] (
			[UserID] INT NOT NULL,
			[Provider] TINYINT NOT NULL,
			[Key] NVARCHAR(1024) NOT NULL,
			CONSTRAINT [PK_dbo.LoginKey] PRIMARY KEY CLUSTERED (
				[UserID] ASC,
				[Provider] ASC
			) WITH (
				PAD_INDEX = OFF,
				STATISTICS_NORECOMPUTE = OFF,
				IGNORE_DUP_KEY = OFF,
				ALLOW_ROW_LOCKS = ON,
				ALLOW_PAGE_LOCKS = ON
			) ON [PRIMARY]
		) ON [PRIMARY];

		ALTER TABLE [dbo].[LoginKey] WITH CHECK
			ADD CONSTRAINT [FK_dbo.LoginKey_dbo.User]
			FOREIGN KEY ([UserID]) REFERENCES [dbo].[User] ([ID])
			ON DELETE CASCADE;

		ALTER TABLE [dbo].[LoginKey] CHECK CONSTRAINT [FK_dbo.LoginKey_dbo.User];

		PRINT '''LoginKey'' table was created'
	END
	ELSE
		PRINT '''LoginKey'' table already exists'

	/* Role Table */
	IF NOT EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'Role')
	BEGIN
		SET ANSI_NULLS ON;
		SET QUOTED_IDENTIFIER ON;

		CREATE TABLE [dbo].[Role] (
			[ID] INT IDENTITY(1,1) NOT NULL,
			[Name] NVARCHAR(128) NOT NULL,
			[Created] DATETIME2(7) NOT NULL,
			CONSTRAINT [PK_dbo.Role] PRIMARY KEY CLUSTERED (
				[ID] ASC
			) WITH (
				PAD_INDEX = OFF,
				STATISTICS_NORECOMPUTE = OFF,
				IGNORE_DUP_KEY = OFF,
				ALLOW_ROW_LOCKS = ON,
				ALLOW_PAGE_LOCKS = ON
			) ON [PRIMARY]
		) ON [PRIMARY];

		PRINT '''Role'' table was created'
	END
	ELSE
		PRINT '''Role'' table already exists'

	/* UserRole Table */
	IF NOT EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserRole')
	BEGIN
		SET ANSI_NULLS ON;
		SET QUOTED_IDENTIFIER ON;

		CREATE TABLE [dbo].[UserRole] (
			[UserID] INT NOT NULL,
			[RoleID] INT NOT NULL,
			CONSTRAINT [PK_dbo.UserRole] PRIMARY KEY CLUSTERED (
				[UserID] ASC,
				[RoleID] ASC
			) WITH (
				PAD_INDEX = OFF,
				STATISTICS_NORECOMPUTE = OFF,
				IGNORE_DUP_KEY = ON,
				ALLOW_ROW_LOCKS = ON,
				ALLOW_PAGE_LOCKS = ON
			) ON [PRIMARY]
		) ON [PRIMARY];

		PRINT '''UserRole'' table was created'
	END
	ELSE
		PRINT '''UserRole'' table already exists'

	/* UserSession Table */
	IF NOT EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserSession')
	BEGIN
		SET ANSI_NULLS ON;
		SET QUOTED_IDENTIFIER ON;

		CREATE TABLE [dbo].[UserSession] (
			[ID] INT IDENTITY(1,1) NOT NULL,
			[UserID] INT NOT NULL,
			[Created] DATETIME2(7) NOT NULL,
			[Token] NVARCHAR(256) NOT NULL,
			[Address] NVARCHAR(128) NOT NULL,
			[Hostname] NVARCHAR(512) NOT NULL,
			CONSTRAINT [PK_dbo.UserSession] PRIMARY KEY CLUSTERED (
				[ID] ASC
			) WITH (
				PAD_INDEX = OFF,
				STATISTICS_NORECOMPUTE = OFF,
				IGNORE_DUP_KEY = OFF,
				ALLOW_ROW_LOCKS = ON,
				ALLOW_PAGE_LOCKS = ON
			) ON [PRIMARY]
		) ON [PRIMARY];

		ALTER TABLE [dbo].[UserSession] WITH CHECK
			ADD CONSTRAINT [FK_dbo.UserSession_dbo.User]
			FOREIGN KEY ([UserID]) REFERENCES [dbo].[User] ([ID])
			ON DELETE CASCADE;

		ALTER TABLE [dbo].[UserSession] CHECK CONSTRAINT [FK_dbo.UserSession_dbo.User];

		PRINT '''UserSession'' table was created'
	END
	ELSE
		PRINT '''UserSession'' table already exists'

	/* UserAuthHistory Table */
	IF NOT EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserAuthHistory')
	BEGIN
		SET ANSI_NULLS ON;
		SET QUOTED_IDENTIFIER ON;

		CREATE TABLE [dbo].[UserAuthHistory] (
			[UserID] INT NOT NULL,
			[Recorded] DATETIME2(7) NOT NULL,
			[Action] TINYINT NOT NULL,
			[Address] NVARCHAR(128) NOT NULL,
			[Hostname] NVARCHAR(512) NOT NULL,
			[Notes] NVARCHAR(512) NOT NULL
		);

		PRINT '''UserAuthHistory'' table was created'
	END
	ELSE
		PRINT '''UserAuthHistory'' table already exists'

	/* UserToken Table */
	IF NOT EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserToken')
	BEGIN
		SET ANSI_NULLS ON;
		SET QUOTED_IDENTIFIER ON;

		CREATE TABLE [dbo].[UserToken] (
			[ID] INT IDENTITY(1,1) NOT NULL,
			[UserID] INT NOT NULL,
			[Created] DATETIME2(7) NOT NULL,
			[Context] NVARCHAR(MAX) NOT NULL,
			[Token] NVARCHAR(256) NOT NULL,
			CONSTRAINT [PK_dbo.UserToken] PRIMARY KEY CLUSTERED (
				[ID] ASC
			) WITH (
				PAD_INDEX = OFF,
				STATISTICS_NORECOMPUTE = OFF,
				IGNORE_DUP_KEY = OFF,
				ALLOW_ROW_LOCKS = ON,
				ALLOW_PAGE_LOCKS = ON
			) ON [PRIMARY]
		) ON [PRIMARY];

		ALTER TABLE [dbo].[UserToken] WITH CHECK
			ADD CONSTRAINT [FK_dbo.UserToken_dbo.User]
			FOREIGN KEY ([UserID]) REFERENCES [dbo].[User] ([ID])
			ON DELETE CASCADE;

		ALTER TABLE [dbo].[UserToken] CHECK CONSTRAINT [FK_dbo.UserToken_dbo.User];

		PRINT '''UserToken'' table was created'
	END
	ELSE
		PRINT '''UserToken'' table already exists'

	/* UserProfile Table */
	IF NOT EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserProfile')
	BEGIN
		SET ANSI_NULLS ON;
		SET QUOTED_IDENTIFIER ON;

		CREATE TABLE [dbo].[UserProfile] (
			[UserID] INT NOT NULL,
			[DisplayName] NVARCHAR(128) NOT NULL,
			[Birthday] DATETIME2(7) NULL,
			[RealName] NVARCHAR(128) NULL,
			[Description] NVARCHAR(4000) NULL,
			[Gender] TINYINT NULL,
			CONSTRAINT [PK_dbo.UserProfile] PRIMARY KEY CLUSTERED (
				[UserID] ASC
			) WITH (
				PAD_INDEX = OFF,
				STATISTICS_NORECOMPUTE = OFF,
				IGNORE_DUP_KEY = OFF,
				ALLOW_ROW_LOCKS = ON,
				ALLOW_PAGE_LOCKS = ON
			) ON [PRIMARY]
		) ON [PRIMARY];

		ALTER TABLE [dbo].[UserProfile] WITH CHECK
			ADD CONSTRAINT [FK_dbo.UserProfile_dbo.User]
			FOREIGN KEY ([UserID]) REFERENCES [dbo].[User] ([ID])
			ON DELETE CASCADE;

		ALTER TABLE [dbo].[UserProfile] CHECK CONSTRAINT [FK_dbo.UserProfile_dbo.User];

		PRINT '''UserProfile'' table was created'
	END
	ELSE
		PRINT '''UserProfile'' table already exists'

	/* UserSettings Table */
	IF NOT EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserSettings')
	BEGIN
		SET ANSI_NULLS ON;
		SET QUOTED_IDENTIFIER ON;

		CREATE TABLE [dbo].[UserSettings] (
			[UserID] INT NOT NULL,
			[HtmlEmails] BIT NOT NULL DEFAULT((0)),
			[PlaySounds] BIT NOT NULL DEFAULT((0)),
			CONSTRAINT [PK_dbo.UserSettings] PRIMARY KEY CLUSTERED (
				[UserID] ASC
			) WITH (
				PAD_INDEX = OFF,
				STATISTICS_NORECOMPUTE = OFF,
				IGNORE_DUP_KEY = OFF,
				ALLOW_ROW_LOCKS = ON,
				ALLOW_PAGE_LOCKS = ON
			) ON [PRIMARY]
		) ON [PRIMARY];

		ALTER TABLE [dbo].[UserSettings] WITH CHECK
			ADD CONSTRAINT [FK_dbo.UserSettings_dbo.User]
			FOREIGN KEY ([UserID]) REFERENCES [dbo].[User] ([ID])
			ON DELETE CASCADE;

		ALTER TABLE [dbo].[UserSettings] CHECK CONSTRAINT [FK_dbo.UserSettings_dbo.User];

		PRINT '''UserSettings'' table was created'
	END
	ELSE
		PRINT '''UserSettings'' table already exists'

	/* UserVisibilities Table */
	IF NOT EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserVisibilities')
	BEGIN
		SET ANSI_NULLS ON;
		SET QUOTED_IDENTIFIER ON;

		CREATE TABLE [dbo].[UserVisibilities] (
			[UserID] INT NOT NULL,
			[Profile] TINYINT NOT NULL DEFAULT((0)),
			[Email] TINYINT NOT NULL DEFAULT((0)),
			[Searches] TINYINT NOT NULL DEFAULT((0)),
			[Birthday] TINYINT NOT NULL DEFAULT((0)),
			[RealName] TINYINT NOT NULL DEFAULT((0)),
			[Description] TINYINT NOT NULL DEFAULT((0)),
			[Gender] TINYINT NOT NULL DEFAULT((0)),
			CONSTRAINT [PK_dbo.UserVisibilities] PRIMARY KEY CLUSTERED (
				[UserID] ASC
			) WITH (
				PAD_INDEX = OFF,
				STATISTICS_NORECOMPUTE = OFF,
				IGNORE_DUP_KEY = OFF,
				ALLOW_ROW_LOCKS = ON,
				ALLOW_PAGE_LOCKS = ON
			) ON [PRIMARY]
		) ON [PRIMARY];

		ALTER TABLE [dbo].[UserVisibilities] WITH CHECK
			ADD CONSTRAINT [FK_dbo.UserVisibilities_dbo.User]
			FOREIGN KEY ([UserID]) REFERENCES [dbo].[User] ([ID])
			ON DELETE CASCADE;

		ALTER TABLE [dbo].[UserVisibilities] CHECK CONSTRAINT [FK_dbo.UserVisibilities_dbo.User];

		PRINT '''UserVisibilities'' table was created'
	END
	ELSE
		PRINT '''UserVisibilities'' table already exists'

	/* UserContact Table */
	IF NOT EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserContact')
	BEGIN
		SET ANSI_NULLS ON;
		SET QUOTED_IDENTIFIER ON;

		CREATE TABLE [dbo].[UserContact] (
			[UserID] INT NOT NULL,
			[Created] DATETIME2(7) NOT NULL,
			[Type] TINYINT NOT NULL,
			[Value] NVARCHAR(512) NOT NULL,
			[Primary] BIT NOT NULL,
			CONSTRAINT [PK_dbo.UserContact] PRIMARY KEY CLUSTERED (
				[UserID] ASC
			) WITH (
				PAD_INDEX = OFF,
				STATISTICS_NORECOMPUTE = OFF,
				IGNORE_DUP_KEY = OFF,
				ALLOW_ROW_LOCKS = ON,
				ALLOW_PAGE_LOCKS = ON
			) ON [PRIMARY]
		) ON [PRIMARY];

		ALTER TABLE [dbo].[UserContact] WITH CHECK
			ADD CONSTRAINT [FK_dbo.UserContact_dbo.User]
			FOREIGN KEY ([UserID]) REFERENCES [dbo].[User] ([ID])
			ON DELETE CASCADE;

		ALTER TABLE [dbo].[UserContact] CHECK CONSTRAINT [FK_dbo.UserContact_dbo.User];

		PRINT '''UserContact'' table was created'
	END
	ELSE
		PRINT '''UserContact'' table already exists'

	/* UserRelation Table */
	IF NOT EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserRelation')
	BEGIN
		SET ANSI_NULLS ON;
		SET QUOTED_IDENTIFIER ON;

		CREATE TABLE [dbo].[UserRelation] (
			[UserID_One] INT NOT NULL,
			[UserID_Two] INT NOT NULL,
			[Created] DATETIME2(7) NOT NULL,
			[Stage] TINYINT NOT NULL DEFAULT((0)),
			[Origin] BIT NOT NULL DEFAULT((0)),
			CONSTRAINT [PK_dbo.UserRelation] PRIMARY KEY CLUSTERED (
				[UserID_One] ASC,
				[UserID_Two] ASC
			) WITH (
				PAD_INDEX = OFF,
				STATISTICS_NORECOMPUTE = OFF,
				IGNORE_DUP_KEY = OFF,
				ALLOW_ROW_LOCKS = ON,
				ALLOW_PAGE_LOCKS = ON
			) ON [PRIMARY]
		) ON [PRIMARY];

		ALTER TABLE [dbo].[UserRelation] WITH CHECK
			ADD CONSTRAINT [FK_dbo.UserRelation_dbo.User_1]
			FOREIGN KEY ([UserID_One]) REFERENCES [dbo].[User] ([ID])
			ON DELETE CASCADE;

		ALTER TABLE [dbo].[UserRelation] CHECK CONSTRAINT [FK_dbo.UserRelation_dbo.User_1];

		ALTER TABLE [dbo].[UserRelation] WITH CHECK
			ADD CONSTRAINT [FK_dbo.UserRelation_dbo.User_2]
			FOREIGN KEY ([UserID_Two]) REFERENCES [dbo].[User] ([ID])
			ON DELETE NO ACTION;

		ALTER TABLE [dbo].[UserRelation] CHECK CONSTRAINT [FK_dbo.UserRelation_dbo.User_2];

		PRINT '''UserRelation'' table was created'
	END
	ELSE
		PRINT '''UserRelation'' table already exists'

	/* UserRelationEvent Table */
	IF NOT EXISTS(SELECT * FROM [INFORMATION_SCHEMA].[TABLES] WHERE [TABLE_SCHEMA] = 'dbo' AND [TABLE_NAME] = 'UserRelationEvent')
	BEGIN
		SET ANSI_NULLS ON;
		SET QUOTED_IDENTIFIER ON;

		CREATE TABLE [dbo].[UserRelationEvent] (
			[UserID_One] INT NOT NULL,
			[UserID_Two] INT NOT NULL,
			[Recorded] DATETIME2(7) NOT NULL,
			[Stage] TINYINT NOT NULL,
			[Action] TINYINT NOT NULL,
			[Notes] NVARCHAR(512) NOT NULL
		);

		PRINT '''UserRelationEvent'' table was created'
	END
	ELSE
		PRINT '''UserRelationEvent'' table already exists'

	COMMIT
END TRY
BEGIN CATCH
	PRINT 'There was an error in the script, rolling back'
	ROLLBACK

	SELECT ERROR_NUMBER() AS ErrorNumber, ERROR_LINE() AS ErrorLine, ERROR_MESSAGE() AS ErrorMessage
END CATCH