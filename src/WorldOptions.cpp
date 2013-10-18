#include "WorldOptions.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QVariant>

// Write out to a string, used by the File->Save command
QString WorldOptions::writeToXml()
{
    // Generate XML file
    QString output;
    QXmlStreamWriter xml(&output);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("GaiascapeWorld");
    xml.writeTextElement("TerrainSeed", QVariant(terrainSeed).toString());
    xml.writeTextElement("TerrainRandomFactor", QVariant(terrainRandomFactor).toString());
    xml.writeTextElement("TerrainScale", QVariant(terrainScale).toString());
    xml.writeTextElement("ErosionPasses", QVariant(erosionPasses).toString());
    xml.writeTextElement("TerrainSize", QVariant(terrainSize).toString());
    xml.writeTextElement("WorldSize", QVariant(worldSize).toString());
    for(unsigned int layer = 0; layer < 3; layer++)
    {
        xml.writeStartElement("TextureLayer");
        xml.writeAttribute("index", QVariant(layer).toString());
        xml.writeTextElement("DiffuseSpecular", layers[layer].diffuseSpecular);
        xml.writeTextElement("NormalHeight", layers[layer].normalHeight);
        xml.writeTextElement("TextureScale", QVariant(layers[layer].textureScale).toString());
        xml.writeTextElement("MinHeight", QVariant(layers[layer].minHeight).toString());
        xml.writeTextElement("FadeDist", QVariant(layers[layer].fadeDist).toString());
        xml.writeEndElement();
    }
    xml.writeTextElement("SkyDome", skyDome);
    xml.writeTextElement("SkyCurvature", QVariant(skyCurvature).toString());
    xml.writeTextElement("SkyTiling", QVariant(skyTiling).toString());
    xml.writeTextElement("FoliageDensity", QVariant(foliageDensisty).toString());
    xml.writeEndElement(); // GaiascapeWorld
    xml.writeEndDocument();

    return output;
}

// Populate all member variables from the contents of an Xml file created by writeToXml, used by the File->Open command
bool WorldOptions::loadAttributesFromXml(QString xml)
{
    QXmlStreamReader stream(xml);
    while(!stream.atEnd() && !stream.hasError())
    {
        QXmlStreamReader::TokenType token = stream.readNext();

        if(token == QXmlStreamReader::StartDocument)
        {
            continue;
        }
        else if(token == QXmlStreamReader::StartElement)
        {
            if(stream.name() == "GaiascapeWorld")
            {
                continue;
            }
            else if(stream.name() == "TerrainSeed")
            {
                while(token != QXmlStreamReader::EndElement)
                {
                    token = stream.readNext();
                    if(token == QXmlStreamReader::Characters)
                    {
                        terrainSeed = stream.text().toString().toInt();
                    }
                }
            }
            else if(stream.name() == "TerrainRandomFactor")
            {
                while(token != QXmlStreamReader::EndElement)
                {
                    token = stream.readNext();
                    if(token == QXmlStreamReader::Characters)
                    {
                        terrainRandomFactor = stream.text().toString().toInt();
                    }
                }
            }
            else if(stream.name() == "TerrainScale")
            {
                while(token != QXmlStreamReader::EndElement)
                {
                    token = stream.readNext();
                    if(token == QXmlStreamReader::Characters)
                    {
                        terrainScale = stream.text().toString().toInt();
                    }
                }
            }
            else if(stream.name() == "ErosionPasses")
            {
                while(token != QXmlStreamReader::EndElement)
                {
                    token = stream.readNext();
                    if(token == QXmlStreamReader::Characters)
                    {
                        erosionPasses = stream.text().toString().toInt();
                    }
                }
            }
            else if(stream.name() == "TerrainSize")
            {
                while(token != QXmlStreamReader::EndElement)
                {
                    token = stream.readNext();
                    if(token == QXmlStreamReader::Characters)
                    {
                        terrainSize = stream.text().toString().toInt();
                    }
                }
            }
            else if(stream.name() == "WorldSize")
            {
                while(token != QXmlStreamReader::EndElement)
                {
                    token = stream.readNext();
                    if(token == QXmlStreamReader::Characters)
                    {
                        worldSize = stream.text().toString().toInt();
                    }
                }
            }
            else if(stream.name() == "TextureLayer")
            {
                if(!stream.attributes().hasAttribute("index")) return false;
                const int currentLayer = stream.attributes().value("index").toString().toInt();
                while(!(stream.tokenType() == QXmlStreamReader::EndElement && stream.name() == "TextureLayer"))
                {
                    if(stream.name() == "DiffuseSpecular")
                    {
                        while(token != QXmlStreamReader::EndElement)
                        {
                            token = stream.readNext();
                            if(token == QXmlStreamReader::Characters)
                            {
                                layers[currentLayer].diffuseSpecular = stream.text().toString();
                            }
                        }
                    }
                    else if(stream.name() == "NormalHeight")
                    {
                        while(token != QXmlStreamReader::EndElement)
                        {
                            token = stream.readNext();
                            if(token == QXmlStreamReader::Characters)
                            {
                                layers[currentLayer].normalHeight = stream.text().toString();
                            }
                        }
                    }
                    else if(stream.name() == "TextureScale")
                    {
                        while(token != QXmlStreamReader::EndElement)
                        {
                            token = stream.readNext();
                            if(token == QXmlStreamReader::Characters)
                            {
                                layers[currentLayer].textureScale = stream.text().toString().toInt();
                            }
                        }
                    }
                    else if(stream.name() == "MinHeight")
                    {
                        while(token != QXmlStreamReader::EndElement)
                        {
                            token = stream.readNext();
                            if(token == QXmlStreamReader::Characters)
                            {
                                layers[currentLayer].minHeight = stream.text().toString().toDouble();
                            }
                        }
                    }
                    else if(stream.name() == "FadeDist")
                    {
                        while(token != QXmlStreamReader::EndElement)
                        {
                            token = stream.readNext();
                            if(token == QXmlStreamReader::Characters)
                            {
                                layers[currentLayer].fadeDist = stream.text().toString().toDouble();
                            }
                        }
                    }
                    token = stream.readNext();
                }
            }
            else if(stream.name() == "SkyDome")
            {
                while(token != QXmlStreamReader::EndElement)
                {
                    token = stream.readNext();
                    if(token == QXmlStreamReader::Characters)
                    {
                        skyDome = stream.text().toString();
                    }
                }
            }
            else if(stream.name() == "SkyCurvature")
            {
                while(token != QXmlStreamReader::EndElement)
                {
                    token = stream.readNext();
                    if(token == QXmlStreamReader::Characters)
                    {
                        skyCurvature = stream.text().toString().toDouble();
                    }
                }
            }
            else if(stream.name() == "SkyTiling")
            {
                while(token != QXmlStreamReader::EndElement)
                {
                    token = stream.readNext();
                    if(token == QXmlStreamReader::Characters)
                    {
                        skyTiling = stream.text().toString().toInt();
                    }
                }
            }
            else if(stream.name() == "FoliageDensity")
            {
                while(token != QXmlStreamReader::EndElement)
                {
                    token = stream.readNext();
                    if(token == QXmlStreamReader::Characters)
                    {
                        foliageDensisty = stream.text().toString().toInt();
                    }
                }
            }
        }
    }
    if(stream.hasError()) return false;
    else return true;
}
