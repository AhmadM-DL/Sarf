-- MySQL dump 10.11
--
-- Host: localhost    Database: atm
-- ------------------------------------------------------
-- Server version	5.0.67-0ubuntu6

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `Categories`
--

DROP TABLE IF EXISTS `Categories`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `Categories` (
  `id` int(11) NOT NULL default '0',
  `name` varchar(40) NOT NULL,
  `type` bit(2) NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `Categories`
--

LOCK TABLES `Categories` WRITE;
/*!40000 ALTER TABLE `Categories` DISABLE KEYS */;
/*!40000 ALTER TABLE `Categories` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `affixes`
--

DROP TABLE IF EXISTS `affixes`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `affixes` (
  `id` int(11) NOT NULL auto_increment,
  `tr_affix` varchar(10) default NULL,
  `ar_affix` varchar(10) NOT NULL,
  `is_prefix` bit(1) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `affixes`
--

LOCK TABLES `affixes` WRITE;
/*!40000 ALTER TABLE `affixes` DISABLE KEYS */;
/*!40000 ALTER TABLE `affixes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `category_suffix`
--

DROP TABLE IF EXISTS `category_suffix`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `category_suffix` (
  `category_id` int(11) NOT NULL,
  `suffix_id` int(11) NOT NULL,
  `source_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`suffix_id`,`category_id`,`source_id`),
  KEY `category_id` (`category_id`),
  KEY `source_id` (`source_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `category_suffix`
--

LOCK TABLES `category_suffix` WRITE;
/*!40000 ALTER TABLE `category_suffix` DISABLE KEYS */;
/*!40000 ALTER TABLE `category_suffix` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `main`
--

DROP TABLE IF EXISTS `main`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `main` (
  `id` bigint(20) NOT NULL auto_increment,
  `ar_key` varchar(30) default NULL,
  `tr_key` varchar(30) default NULL,
  `stem` varchar(30) default NULL,
  `category` bigint(20) unsigned NOT NULL,
  `source_id` int(11) default NULL,
  `raw_form` varchar(60) default NULL,
  PRIMARY KEY  (`id`),
  KEY `source_id` (`source_id`)
) ENGINE=MyISAM AUTO_INCREMENT=31 DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `main`
--

LOCK TABLES `main` WRITE;
/*!40000 ALTER TABLE `main` DISABLE KEYS */;
INSERT INTO `main` VALUES (27,'Ø§Ø³Ù…','','',18446744073709551615,1,''),(28,'Ø§Ø³Ù…','','',0,1,''),(29,'Ø§Ø³Ù…','','',0,1,''),(30,'Ø§Ø³Ù…','','',0,1,'');
/*!40000 ALTER TABLE `main` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `prefixCategories`
--

DROP TABLE IF EXISTS `prefixCategories`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `prefixCategories` (
  `id` int(11) NOT NULL default '0',
  `name` varchar(30) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `prefixCategories`
--

LOCK TABLES `prefixCategories` WRITE;
/*!40000 ALTER TABLE `prefixCategories` DISABLE KEYS */;
/*!40000 ALTER TABLE `prefixCategories` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `prefix_category`
--

DROP TABLE IF EXISTS `prefix_category`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `prefix_category` (
  `prefix_id` int(11) NOT NULL,
  `category_id` int(11) NOT NULL,
  `source_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`prefix_id`,`category_id`,`source_id`),
  KEY `category_id` (`category_id`),
  KEY `source_id` (`source_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `prefix_category`
--

LOCK TABLES `prefix_category` WRITE;
/*!40000 ALTER TABLE `prefix_category` DISABLE KEYS */;
/*!40000 ALTER TABLE `prefix_category` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `prefix_prefix`
--

DROP TABLE IF EXISTS `prefix_prefix`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `prefix_prefix` (
  `prefix_id1` int(11) NOT NULL,
  `prefix_id2` int(11) NOT NULL,
  `source_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`prefix_id1`,`prefix_id2`,`source_id`),
  KEY `prefix_id2` (`prefix_id2`),
  KEY `source_id` (`source_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `prefix_prefix`
--

LOCK TABLES `prefix_prefix` WRITE;
/*!40000 ALTER TABLE `prefix_prefix` DISABLE KEYS */;
/*!40000 ALTER TABLE `prefix_prefix` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `prefix_suffix`
--

DROP TABLE IF EXISTS `prefix_suffix`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `prefix_suffix` (
  `prefix_id` int(11) NOT NULL,
  `suffix_id` int(11) NOT NULL,
  `source_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`prefix_id`,`suffix_id`,`source_id`),
  KEY `suffix_id` (`suffix_id`),
  KEY `source_id` (`source_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `prefix_suffix`
--

LOCK TABLES `prefix_suffix` WRITE;
/*!40000 ALTER TABLE `prefix_suffix` DISABLE KEYS */;
/*!40000 ALTER TABLE `prefix_suffix` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `prefixes`
--

DROP TABLE IF EXISTS `prefixes`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `prefixes` (
  `id` int(11) NOT NULL default '0',
  `name` char(5) NOT NULL,
  `category_id` int(11) default NULL,
  `sources` bigint(20) unsigned NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `name` (`name`,`category_id`),
  KEY `category_id` (`category_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `prefixes`
--

LOCK TABLES `prefixes` WRITE;
/*!40000 ALTER TABLE `prefixes` DISABLE KEYS */;
/*!40000 ALTER TABLE `prefixes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `rules`
--

DROP TABLE IF EXISTS `rules`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `rules` (
  `prefix` int(11) NOT NULL default '0',
  `stem` int(11) NOT NULL default '0',
  `suffix` int(11) NOT NULL default '0',
  PRIMARY KEY  (`prefix`,`stem`,`suffix`),
  KEY `stem` (`stem`),
  KEY `suffix` (`suffix`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `rules`
--

LOCK TABLES `rules` WRITE;
/*!40000 ALTER TABLE `rules` DISABLE KEYS */;
/*!40000 ALTER TABLE `rules` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `semantical_categories`
--

DROP TABLE IF EXISTS `semantical_categories`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `semantical_categories` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(256) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `semantical_categories`
--

LOCK TABLES `semantical_categories` WRITE;
/*!40000 ALTER TABLE `semantical_categories` DISABLE KEYS */;
INSERT INTO `semantical_categories` VALUES (3,'Ø§Ø³Ù…');
/*!40000 ALTER TABLE `semantical_categories` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sources`
--

DROP TABLE IF EXISTS `sources`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `sources` (
  `id` int(11) NOT NULL auto_increment,
  `source` varchar(1000) default NULL,
  `normalization_process` varchar(1000) default NULL,
  `creator` varchar(50) default NULL,
  `date_start` date default NULL,
  `date_last` date default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `sources`
--

LOCK TABLES `sources` WRITE;
/*!40000 ALTER TABLE `sources` DISABLE KEYS */;
INSERT INTO `sources` VALUES (1,'jad',NULL,NULL,NULL,'2009-12-03'),(2,'hamza','','','2009-12-03','2009-12-03');
/*!40000 ALTER TABLE `sources` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `stems`
--

DROP TABLE IF EXISTS `stems`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `stems` (
  `id` bigint(20) unsigned NOT NULL auto_increment,
  `name` varchar(30) NOT NULL,
  `grammar_stem` char(10) default NULL,
  `category_id` int(11) NOT NULL,
  `semantical_category` int(11) NOT NULL,
  `sources` bigint(20) unsigned NOT NULL,
  `diactric_name` varchar(60) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `name` (`name`,`category_id`),
  KEY `category_id` (`category_id`),
  KEY `semantical_category` (`semantical_category`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `stems`
--

LOCK TABLES `stems` WRITE;
/*!40000 ALTER TABLE `stems` DISABLE KEYS */;
/*!40000 ALTER TABLE `stems` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `suffix_suffix`
--

DROP TABLE IF EXISTS `suffix_suffix`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `suffix_suffix` (
  `suffix_id1` int(11) NOT NULL,
  `suffix_id2` int(11) NOT NULL,
  `source_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`suffix_id1`,`suffix_id2`,`source_id`),
  KEY `suffix_id2` (`suffix_id2`),
  KEY `source_id` (`source_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `suffix_suffix`
--

LOCK TABLES `suffix_suffix` WRITE;
/*!40000 ALTER TABLE `suffix_suffix` DISABLE KEYS */;
/*!40000 ALTER TABLE `suffix_suffix` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `suffixes`
--

DROP TABLE IF EXISTS `suffixes`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `suffixes` (
  `id` int(11) NOT NULL default '0',
  `name` char(5) NOT NULL,
  `category_id` int(11) default NULL,
  `sources` bigint(20) unsigned NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `name` (`name`,`category_id`),
  KEY `category_id` (`category_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `suffixes`
--

LOCK TABLES `suffixes` WRITE;
/*!40000 ALTER TABLE `suffixes` DISABLE KEYS */;
/*!40000 ALTER TABLE `suffixes` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2009-12-03 17:32:33