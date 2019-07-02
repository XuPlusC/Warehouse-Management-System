DROP TABLE IF EXISTS LACK_LOG
DROP TABLE IF EXISTS SHIPMENT_LOG
DROP TABLE IF EXISTS SHIPMENT
DROP TABLE IF EXISTS PURCHASE_LOG
DROP TABLE IF EXISTS PURCHASE
DROP TABLE IF EXISTS SELLER
DROP TABLE IF EXISTS BUYER
DROP TABLE IF EXISTS GOODS
DROP TABLE IF EXISTS ADMIN

CREATE TABLE GOODS
(
	goodsID int PRIMARY KEY,
	goodsName char(30),
	storageNum int,
	place char(30),
	producer char(30)
);
GO

CREATE TABLE ADMIN
(
	adminID int NOT NULL PRIMARY KEY,
	adminName char(30),
	password char(6) NOT NULL
);
GO

CREATE TABLE BUYER
(
	buyerID int NOT NULL PRIMARY KEY,
	buyerName char(30),
	password char(6) NOT NULL
);
GO

CREATE TABLE SELLER
(
	sellerID int NOT NULL PRIMARY KEY,
	sellerName char(30),
	password char(6) NOT NULL
);
GO

CREATE TABLE PURCHASE
(
	purchaseID int NOT NULL PRIMARY KEY,
	buyerID int,
	time datetime,
	status char(1),
	adminID int,
	CONSTRAINT fk_P_buyer FOREIGN KEY (buyerID) REFERENCES BUYER(buyerID),
	CONSTRAINT fk_P_admin FOREIGN KEY (adminID) REFERENCES ADMIN(adminID)
);
GO

CREATE TABLE PURCHASE_LOG
(
	purchaseLogID int NOT NULL PRIMARY KEY,
	purchaseID int,
	goodsID int,
	goodsName char(30),
	num int,
	CONSTRAINT chk_PL_num CHECK (num >= 0),
	CONSTRAINT fk_PL_pid FOREIGN KEY (purchaseID) REFERENCES PURCHASE(purchaseID),
	CONSTRAINT fk_PL_gid FOREIGN KEY (goodsID) REFERENCES GOODS(goodsID)
);
GO

CREATE TABLE SHIPMENT
(
	shipmentID int NOT NULL PRIMARY KEY,
	sellerID int,
    time datetime,
	status char(1),
	adminID int
	CONSTRAINT fk_S_seller FOREIGN KEY (sellerID) REFERENCES SELLER(sellerID),
	CONSTRAINT fk_S_admin FOREIGN KEY (adminID) REFERENCES ADMIN(adminID)
);
GO

CREATE TABLE SHIPMENT_LOG
(
	shipmentLogID int NOT NULL PRIMARY KEY,
	shipmentID int, 
	goodsID int,
	goodsName char(30),
	num int,
	CONSTRAINT chk_SL_num CHECK (num >= 0),
	CONSTRAINT fk_SL_sid FOREIGN KEY (shipmentID) REFERENCES SHIPMENT(shipmentID),
	CONSTRAINT fk_SL_gid FOREIGN KEY (goodsID) REFERENCES GOODS(goodsID)
);
GO

CREATE TABLE LACK_LOG
(
	lackLogID int NOT NULL PRIMARY KEY,
	shipmentID int,
	goodsID int,
	goodsName char(30), 
	lackNum int,
	isSatisfied char(1), --- 0 不可满足 1 可满足 2 已满足
	time datetime,
	CONSTRAINT fk_LL_sid FOREIGN KEY (shipmentID) REFERENCES SHIPMENT(shipmentID),
	CONSTRAINT fk_LL_gid FOREIGN KEY (goodsID) REFERENCES GOODS(goodsID)
);
GO

INSERT INTO ADMIN
VALUES(1, '管_张三', '123456');

INSERT INTO BUYER
VALUES(1, '购_张三', '123456');

INSERT INTO SELLER
VALUES(1, '销_张三', '123456');

INSERT INTO GOODS
VALUES(1,'书', 100, '武汉', '武汉出版社');

INSERT INTO GOODS
VALUES(2, '笔记本', 200, '武汉', '印刷厂');

INSERT INTO PURCHASE
VALUES(1, 1, '2014-12-03 17:06:15.433', 0, NULL);

INSERT INTO PURCHASE
VALUES(2, 1, '2014-12-03 17:06:15.433', 0, NULL);

INSERT INTO PURCHASE_LOG
VALUES(1, 1, 1, '书', 200);
INSERT INTO PURCHASE_LOG
VALUES(2, 1, 2, '笔记本', 10);
INSERT INTO PURCHASE_LOG
VALUES(3, 2, 1, '书', 20);
INSERT INTO PURCHASE_LOG
VALUES(4, 2, 2, '笔记本', 20);

INSERT INTO SHIPMENT
VALUES(1, 1, '2014-12-03 17:06:15.433', 0, NULL);

INSERT INTO SHIPMENT_LOG
VALUES(1, 1, 1, '书', 200);

DROP TRIGGER IF EXISTS TRGR_GOODS_NAMECHANGE;
GO
CREATE TRIGGER TRGR_GOODS_NAMECHANGE
ON GOODS
AFTER UPDATE
AS BEGIN
	declare @goodsID int
	declare @new_goodsName char(30)
	declare @new_storageNum int
	
	select @goodsID = goodsID from inserted
	select @new_goodsName = goodsName from inserted
	select @new_storageNum = storageNum from inserted
	
	update PURCHASE_LOG set goodsName = @new_goodsName where goodsID = @goodsID
	update SHIPMENT_LOG set goodsName = @new_goodsName where goodsID = @goodsID
	update LACK_LOG set goodsName = @new_goodsName where goodsID = @goodsID
	
	update LACK_LOG
	set isSatisfied = '1'
	where goodsID = @goodsID and isSatisfied = '0' and @new_storageNum >= lackNum
	
	update LACK_LOG
	set isSatisfied = '0'
	where goodsID = @goodsID and isSatisfied = '1' and @new_storageNum < lackNum
END

DROP TRIGGER IF EXISTS TRGR_GOODS_CHECKSAT;
/*
GO
CREATE TRIGGER TRGR_GOODS_CHECKSAT
ON GOODS
AFTER UPDATE
AS BEGIN
	declare @goodsID int

	select @goodsID = goodsID from inserted
	
	update LACK_LOG
	set isSatisfied = '1'
	where goodsID = @goodsID and isSatisfied = '0' and @new_storageNum >= lackNum
	
	update LACK_LOG
	set isSatisfied = '0'
	where goodsID = @goodsID and isSatisfied = '1' and @new_storageNum < lackNum
END
*/