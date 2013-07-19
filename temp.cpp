///* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2010 Robert Osfield
// *
// * This application is open source and may be redistributed and/or modified
// * freely and without restriction, both in commercial and non commercial applications,
// * as long as this copyright notice is maintained.
// *
// * This application is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//*/
//




//if(BitsPerSample== 16) {
		//	amplitude = 32767 * amplitude_coefficient;		
		//	if(WaveForm == WaveForm_Sine) {
		//		for(i=0; i< NumSamples; i++) {					
		//			if(frequency_sweep)	{			

		//				//v_wave.push_back(amplitude* sin( 2.0* osg::PI* ((start_freq* sample_time_elapsed)+((chirp_rate/2)*powf(sample_time_elapsed,2)))));

		//				/*n_A = static_cast<int>();	
		//				WriteFile(hFile, &n_A, 2, &dwFileSize, NULL);	*/
		//			}
		//			else {
		//				n_A = static_cast<int>(amplitude* sin((angular_frequency* sample_time_elapsed)+phase));		
		//				WriteFile(hFile, &n_A, 2, &dwFileSize, NULL);
		//			}				
		//			sample_time_elapsed+= sample_period;
		//		}	
		//	}
		//	else if(type == "Cos") {
		//		for(i=0; i< NumSamples; i++) {							
		//			n_A = static_cast<int>(amplitude* cos((angular_frequency* sample_time_elapsed)+phase));		
		//			WriteFile(hFile, &n_A, 2, &dwFileSize, NULL);				
		//			sample_time_elapsed+= sample_period;
		//		}	
		//	}
		//	else if(type == "Fourier") {
		//		for(i=0; i< NumSamples; i++) {						
		//			n_A = static_cast<int>(amplitude* square_wave_fourier_series(angular_frequency* sample_time_elapsed, N));		
		//			WriteFile(hFile, &n_A, 2, &dwFileSize, NULL);				
		//			sample_time_elapsed+= sample_period;
		//		}	
		//	}
		//	else if(type == "AddFrequencies")
		//	{
		//		for(i=0; i< NumSamples; i++) {							
		//			ft=0;
		//			std::vector<double>::iterator lf_iter;
		//			for(lf_iter = v_frequency.begin(); lf_iter < v_frequency.end(); ++lf_iter) {	
		//				angular_frequency= 2* osg::PI* (*lf_iter);
		//				ft += (sin((angular_frequency* sample_time_elapsed)+phase)/ num_freq+1);
		//			}
		//			ft*= amplitude;
		//			int n_ft = (int)ft;						
		//			WriteFile(hFile, &n_ft, 2, &dwFileSize, NULL);
		//			
		//			sample_time_elapsed+= sample_period;
		//		}	
		//	}
		//	else if(type == "Synthesis")
		//	{
		//		stephens_find_max = 1.0/(double)NumSamples;
		//		recip_sampl_accumulator= 0;
		//		stephens_max = stephens_approx_zero_grad(3,4,5,6,ParamC);
		//		stephens_amplitude_coefficient= 1.0/stephens_max;
		//	
		//		for(i=0; i< NumSamples; i++) {							
		//			ft=0;
		//			std::vector<double>::iterator lf_iter;
		//			for(lf_iter = v_frequency.begin(); lf_iter < v_frequency.end(); ++lf_iter) {	
		//				angular_frequency= 2* osg::PI* (*lf_iter);
		//				ft += (sin((angular_frequency* sample_time_elapsed)+phase)/ ct);
		//			}					
		//			stephens= stephens_amplitude_coefficient* stephens_beta(recip_sampl_accumulator, 3,4,5,6,ParamC);					
		//			ft*= (stephens* amplitude);
		//			if(ft>32766){ ft= 32766; }
		//			if(ft<-32766){ ft= -32766; }
		//			int n_ft = (int)ft;						
		//			WriteFile(hFile, &n_ft, 2, &dwFileSize, NULL);
		//		
		//			recip_sampl_accumulator+= num_samp_recip;
		//			sample_time_elapsed+= sample_period;
		//		}	
		//	}
		//}

//if(BitsPerSample== 16) {
//	amplitude = 32767 * amplitude_coefficient;		
//	if(type == "Sine") {
//		for(i=0; i< NumSamples; i++) {					
//			if(frequency_sweep)	{			
//
//				v_wave.push_back(amplitude* sin( 2.0* osg::PI* ((start_freq* sample_time_elapsed)+((chirp_rate/2)*powf(sample_time_elapsed,2)))));
//
//				/*n_A = static_cast<int>();	
//				WriteFile(hFile, &n_A, 2, &dwFileSize, NULL);	*/
//			}
//			else {
//				n_A = static_cast<int>(amplitude* sin((angular_frequency* sample_time_elapsed)+phase));		
//				WriteFile(hFile, &n_A, 2, &dwFileSize, NULL);
//			}				
//			sample_time_elapsed+= sample_period;
//		}	
//	}
//	else if(type == "Cos") {
//		for(i=0; i< NumSamples; i++) {							
//			n_A = static_cast<int>(amplitude* cos((angular_frequency* sample_time_elapsed)+phase));		
//			WriteFile(hFile, &n_A, 2, &dwFileSize, NULL);				
//			sample_time_elapsed+= sample_period;
//		}	
//	}
//	else if(type == "Fourier") {
//		for(i=0; i< NumSamples; i++) {						
//			n_A = static_cast<int>(amplitude* square_wave_fourier_series(angular_frequency* sample_time_elapsed, N));		
//			WriteFile(hFile, &n_A, 2, &dwFileSize, NULL);				
//			sample_time_elapsed+= sample_period;
//		}	
//	}
//	else if(type == "AddFrequencies")
//	{
//		std::size_t count = v_frequency.size();
//		double ct= count+1;
//		for(i=0; i< NumSamples; i++) {							
//			ft=0;
//			std::vector<double>::iterator lf_iter;
//			for(lf_iter = v_frequency.begin(); lf_iter < v_frequency.end(); ++lf_iter) {	
//				angular_frequency= 2* osg::PI* (*lf_iter);
//				ft += (sin((angular_frequency* sample_time_elapsed)+phase)/ ct);
//			}
//			ft*= amplitude;
//			int n_ft = (int)ft;						
//			WriteFile(hFile, &n_ft, 2, &dwFileSize, NULL);
//					
//			sample_time_elapsed+= sample_period;
//		}	
//	}
//	else if(type == "Synthesis")
//	{
//		stephens_find_max = 1.0/(double)NumSamples;
//		recip_sampl_accumulator= 0;
//		stephens_max = stephens_approx_zero_grad(3,4,5,6,ParamC);
//		stephens_amplitude_coefficient= 1.0/stephens_max;
//		std::size_t count = v_frequency.size();
//		double ct= count+1;
//		for(i=0; i< NumSamples; i++) {							
//			ft=0;
//			std::vector<double>::iterator lf_iter;
//			for(lf_iter = v_frequency.begin(); lf_iter < v_frequency.end(); ++lf_iter) {	
//				angular_frequency= 2* osg::PI* (*lf_iter);
//				ft += (sin((angular_frequency* sample_time_elapsed)+phase)/ ct);
//			}					
//			stephens= stephens_amplitude_coefficient* stephens_beta(recip_sampl_accumulator, 3,4,5,6,ParamC);					
//			ft*= (stephens* amplitude);
//			if(ft>32766){ ft= 32766; }
//			if(ft<-32766){ ft= -32766; }
//			int n_ft = (int)ft;						
//			WriteFile(hFile, &n_ft, 2, &dwFileSize, NULL);
//				
//			recip_sampl_accumulator+= stephens_find_max;
//			sample_time_elapsed+= sample_period;
//		}	
//	}
//}

//#include <osgDB/ReadFile>
//#include <osgUtil/Optimizer>
//#include <osg/CoordinateSystemNode>
//
////command arguments
////C:\OpenSceneGraph-3.0.1\samples\OpenSceneGraph-Data-3.0.0\OpenSceneGraph-Data-3.0.0\CESSNA.OSG
//
//
//#include <osg/Switch>
//#include <osg/PositionAttitudeTransform>
//
//#include <osgText/Text>
//
//#include <osgViewer/Viewer>
//#include <osgViewer/ViewerEventHandlers>
//
//#include <osgGA/TrackballManipulator>
//#include <osgGA/FlightManipulator>
//#include <osgGA/DriveManipulator>
//#include <osgGA/KeySwitchMatrixManipulator>
//#include <osgGA/StateSetManipulator>
//#include <osgGA/AnimationPathManipulator>
//#include <osgGA/TerrainManipulator>
//#include <osgGA/SphericalManipulator>
//#include <osgGA/StandardManipulator>
//#include <iostream>
//#include <fstream>
//
//
//osg::ref_ptr<osgText::Font> g_font =
//osgText::readFontFile("fonts/arial.ttf");
//
////osgViewer::Viewer viewer;
//osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;
//osg::Camera* camera;
//
//int const LIGHTS = 3;
//osg::Light* light;
//osg::PositionAttitudeTransform *lightTransform; 
//osg::PositionAttitudeTransform *cubeTransform;
//
//
//char strEyeX[20];
//char strEyeY[20];
//char strEyeZ[20];
//
//char strCentreX[20];
//char strCentreY[20];
//char strCentreZ[20];
//
//char strUpX[20];
//char strUpY[20];
//char strUpZ[20];
//
//
//osg::Vec3d Veye ;
//osg::Vec3d Vcentre ;
//osg::Vec3d Vup ;
//
//int i = 0;
//float zoomFactor = 0.009;
//
//int uniqueLightNumber = 0;
//
//osg::Vec3d Veye1 = osg::Vec3(0,-115.651596,0);
//osg::Vec3d Vcentre1 = osg::Vec3(0,-114.651596,0);
//osg::Vec3d Vup1 = osg::Vec3(0,1,0);
//
//float tempX;
//	
//
//osgGA::OrbitManipulator* orbitManipulator = new osgGA::OrbitManipulator();
//osgGA::TrackballManipulator* trackballManipulator = new osgGA::TrackballManipulator();
//osgGA::TerrainManipulator* terrainManipulator = new osgGA::TerrainManipulator();
//osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;
//
//std::string strManipulator_Name = "";
//std::string strText = "";
//osgViewer::Viewer viewer;
//
//void setCameraHomePosition()
//{
//	Veye = osg::Vec3d(-26814890.040790,29248332.233353,-18638224.923120);
//	Vcentre = osg::Vec3d(-26814889.429176,29248331.566136,-18638224.497973);
//	Vup = osg::Vec3d(0.248953,-0.347769,-0.903924);
//}
//
//void setScene()
//{
//	viewer.getCamera()->setViewMatrixAsLookAt(Veye, Vcentre, Vup);
//}
//
//int writeToFile(std::string text)
//{
//	std::ofstream myfile;
//	myfile.open ("C:\\tmp\\globeCameraCo-ordinates.txt",std::ios_base::app);
//	myfile<<text;
//	myfile.close();
//	return 0;
//}
////osg::Camera* createHUDCamera( float left, float right, float bottom, float top )
//{
//	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
//	camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
//	camera->setClearMask( GL_DEPTH_BUFFER_BIT );
//	camera->setRenderOrder( osg::Camera::POST_RENDER );
//	camera->setAllowEventFocus( false );
//	camera->setProjectionMatrix(osg::Matrix::ortho2D(left, right, bottom, top));
//	return camera.release();
//}
//
//osg::Light* createLight(osg::Vec4 color) 
//{     
//	osg::ref_ptr<osg::Light> light = new osg::Light;
//	//osg::Light *light = new osg::Light();     
//	light->setLightNum(uniqueLightNumber++);     
//	light->setPosition(osg::Vec4(0.0, 0.0, 0.0, 1.0));     
//	light->setDiffuse(color);     
//	light->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));    
//	light->setAmbient( osg::Vec4(0.0, 0.0, 0.0, 1.0));    
//	return light.release(); 
//} 
//
////osg::PositionAttitudeTransform *createlightTransform(void) 
////{     
////	osg::PositionAttitudeTransform *lightTransform = new osg::PositionAttitudeTransform(); 
////	
////	return lightTransform; 
////} 
//
//osgText::Text* createText( const osg::Vec3& pos, const std::string& content, float size )
//{
//	osg::ref_ptr<osgText::Text> text = new osgText::Text;
//	text->setFont( g_font.get() );
//	text->setCharacterSize( size );
//	text->setAxisAlignment( osgText::TextBase::XY_PLANE );
//	text->setPosition( pos );
//	text->setText( content );
//	return text.release();
//}
//
//void readFromFile()
//{
//	int i =0;
//	std::ifstream myReadFile;
//	 myReadFile.open("C:\\tmp\\globeCameraCo-ordinates.txt");
//	 std::string output;
//	 if (myReadFile.is_open()) {
//		 while (!myReadFile.eof()) {
//			myReadFile >> output;
//			textGeode->setDrawable(i,createText(osg::Vec3(50.0f, 200.0f, 0.0f), output ,10.0f));
//			++i;
//		}
//	}
//}
//
//osg::Node *create_light_node() { 	
//	osg::Group *scene = new osg::Group(); 
//	cubeTransform = new osg::PositionAttitudeTransform(); 
//	light = createLight(osg::Vec4(1.0, 1.0, 1.0, 1.0));
//	osg::LightSource *lightSource;
//	lightSource = new osg::LightSource(); 
//	osg::StateSet *lightStateSet;	
//	lightStateSet = scene->getOrCreateStateSet();        
//	osg::Vec4 lightColors = osg::Vec4(1.0, 0.0, 0.0, 1.0);
//	lightSource->setLight(createLight(lightColors));         
//	lightSource->setLocalStateSetModes(osg::StateAttribute::ON);        
//	lightSource->setStateSetModes(*lightStateSet, osg::StateAttribute::ON);  	
//	lightTransform = new osg::PositionAttitudeTransform(); 	   
//	lightTransform->addChild(lightSource);      
//	lightTransform->setPosition(osg::Vec3(0, 0, 5));         
//	lightTransform->setScale(osg::Vec3(0.1,0.1,0.1));         
//	scene->addChild(lightTransform); 
//	return scene; 
//
//}
//int main(int argc, char** argv)
//{
//	//Event Handeling  
//	//ObjectManipulator* displayHandler = new ObjectManipulator(); 
//    //viewer.addEventHandler(displayHandler);   
//    //textGeode->addDrawable( createText( osg::Vec3(150.0f, 500.0f, 0.0f),"",20.0f));
//	
//	//osg::Geode* HUDGeode = new osg::Geode();
//	//osgText::Text* textOne = new osgText::Text();
//	// HUDGeode->addDrawable( textOne );
//
//	camera = createHUDCamera(0, 1024, 0, 768);
//	camera->addChild( textGeode.get() );
//	//camera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF );
//	camera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON );
//
//	osg::ref_ptr<osg::Group> root = new osg::Group;
//	//root->addChild(osgDB::readNodeFile("C:\\Virtual_Geelong\\d\\k2vi\\globes\\Geelong_globe\\globe.ive")); //CPhysXTerrain	
//	osgGA::SphericalManipulator* sphericalManipulator = new osgGA::SphericalManipulator();
//	sphericalManipulator->setAllowThrow(false);		
//	terrainManipulator->setAllowThrow(false);
//	terrainManipulator->setWheelZoomFactor(zoomFactor);		
//	orbitManipulator->setAllowThrow(false);
//	orbitManipulator->setWheelZoomFactor(zoomFactor);	
//    keyswitchManipulator->addMatrixManipulator( '1', "Trackball",trackballManipulator );
//    keyswitchManipulator->addMatrixManipulator( '2', "Flight", new osgGA::FlightManipulator() );
//    keyswitchManipulator->addMatrixManipulator( '3', "Drive", new osgGA::DriveManipulator() );
//    keyswitchManipulator->addMatrixManipulator( '4', "CPhysXTerrain", terrainManipulator );
//    keyswitchManipulator->addMatrixManipulator( '5', "Orbit", orbitManipulator );
//    keyswitchManipulator->addMatrixManipulator( '6', "FirstPerson", new osgGA::FirstPersonManipulator() );
//	keyswitchManipulator->addMatrixManipulator( '7', "Spherical",sphericalManipulator );   
//	setCameraHomePosition();
//	strManipulator_Name = keyswitchManipulator.get()->getName();
//	viewer.setCameraManipulator( keyswitchManipulator.get());
//    viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
//    viewer.addEventHandler(new osgViewer::ThreadingHandler);
//    viewer.addEventHandler(new osgViewer::WindowSizeHandler);
//    viewer.addEventHandler(new osgViewer::StatsHandler);
//    viewer.addEventHandler(new osgViewer::RecordCameraPathHandler);
//    viewer.addEventHandler(new osgViewer::LODScaleHandler);
//    viewer.addEventHandler(new osgViewer::ScreenCaptureHandler);    
//
//	osg::Node *light = create_light_node(); 
//
//	osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile("I:\\media\\architecture\\smith_housez.3DS");	
//    //osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile("I:\\3ds\\A10.3ds");	
//    if (!loadedModel)
//    {
//        std::cout << ": No data loaded" << std::endl;
//        return 1;
//    }
//	osgUtil::Optimizer optimizer;
//    optimizer.optimize(loadedModel.get());
//	root->addChild(loadedModel.get()); //CPhysXTerrain
//
//	
//	root->addChild( camera );	
//    root->addChild( light );	
//    viewer.setSceneData( root.get() );
//    viewer.realize();	    
//    while (!viewer.done())
//    {		
//        viewer.frame();
//    }
//}




//void CEnvironment::XML_Parse_GeometryGroup(CComPtr<IXMLDOMNode> spXMLNode, Group *grp)
//{
//	HRESULT hr;
//	long listLength;
//	GeometryType geo_type = eGeometryTypeUndefined;
//	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
//	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);
//	if ( FAILED(hr) ) 
//		throw "Unable to get node list";		
//	hr =spXMLDOMNodeList->get_length(&listLength);
//	if ( FAILED(hr) ) 
//		throw "Unable to get list length";	
//	osg::ref_ptr<osg::Node> loadedModel = NULL;
//	Geode *geode= NULL;
//	PositionAttitudeTransform *PAT = NULL;
//	for(long i= 0; i< listLength; i++)
//	{
//		string str_node_name;
//		string str_node_text;
//		CComPtr<IXMLDOMNode> spXMLNextNode;
//		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
//		if(spXMLNextNode!= NULL)
//		{
//			if(str_node_name== "Sphere")
//			{
//				geo_type = eGeometryTypeSphere;
//				geode = new Geode();
//				float radius;
//				Vec3d center;
//				XML_Parse_Sphere(spXMLNextNode, &center, &radius);				
//				geode->addDrawable(new ShapeDrawable(new Sphere(center, radius)));
//			}
//			else if(str_node_name== "Box")
//			{
//				geo_type = eGeometryTypeBox;
//				geode = new Geode();
//				float x=1;
//				float y=1;
//				float z=1;
//				Vec3d center;
//				XML_Parse_Box(spXMLNextNode, &center, &x, &y, &z);				
//				geode->addDrawable(new ShapeDrawable(new Box(center, x,y,z)));			
//			}
//			else if(str_node_name== "Capsule")
//			{
//				geo_type = eGeometryTypeCapsule;
//				geode = new Geode();
//				float radius=1;
//				float height=1;
//				Vec3d center;
//				XML_Parse_Capsule(spXMLNextNode, &center, &radius, &height);				
//				geode->addDrawable(new ShapeDrawable(new Capsule(center, radius, height)));			
//			}
//			else if(str_node_name== "Cone")
//			{
//				geo_type = eGeometryTypeCone;
//				geode = new Geode();
//				float radius=1;
//				float height=1;
//				Vec3d center;
//				XML_Parse_Cone(spXMLNextNode, &center, &radius, &height);				
//				geode->addDrawable(new ShapeDrawable(new Cone(center, radius, height)));			
//			}
//			else if(str_node_name== "Cylinder")
//			{
//				geo_type = eGeometryTypeCylinder;
//				geode = new Geode();
//				float radius=1;
//				float height=1;
//				Vec3d center;
//				XML_Parse_Cylinder(spXMLNextNode, &center, &radius, &height);				
//				geode->addDrawable(new ShapeDrawable(new Cylinder(center, radius, height)));			
//			}
//			else if(str_node_name== "HeightField")
//			{
//				geo_type = eGeometryTypeHeightField;
//				string height_file;
//				string texture_file;
//				XML_Parse_HeightField(spXMLNextNode, &height_file, &texture_file);				
//				geode = createHeightField(height_file, texture_file);
//
//				if(geode==NULL)throw "Unable make Height Field";	
//			}			
//			else if(str_node_name== "Cube")
//			{
//				geo_type = eGeometryTypeCube;
//				geode = createUnitCube();				
//			}
//			else if(str_node_name== "FilePath")
//			{
//				geo_type = eGeometryTypeFile;
//				/*This includes the following geometric file formats: 3dc, 3ds, flt, geo, iv, ive, lwo, md2, obj, osg. 
//					And the following image file formats: bmp, gif, jpeg, rgb, tga, tif. */
//
//				loadedModel = osgDB::readNodeFile(str_node_text);	
//				if (!loadedModel)
//				{
//					std::cout << ": No data loaded" << std::endl;
//				}	
//			}	
//			else if(str_node_name == "Attribute")
//			{
//				if((geo_type==eGeometryTypeBox)||(geo_type==eGeometryTypeCapsule)||(geo_type==eGeometryTypeCompositeShape)
//					||(geo_type==eGeometryTypeCone)||(geo_type==eGeometryTypeCylinder)||(geo_type==eGeometryTypeHeightField)
//					||(geo_type==eGeometryTypeInfinitePlane)||(geo_type==eGeometryTypeKdTree)||(geo_type==eGeometryTypeSphere)
//					||(geo_type==eGeometryTypTriangleMesh)||(geo_type==eGeometryTypeCube)
//					)
//				{
//					if(geode!= NULL)
//					{
//						XML_Parse_Attribute(spXMLNextNode, geode);
//					}
//				}
//				else if (geo_type==eGeometryTypeFile)
//				{
//					if(loadedModel!= NULL)
//					{
//						XML_Parse_Attribute(spXMLNextNode, loadedModel.get());
//					}
//				}				
//			}			
//			else if(str_node_name == "PositionAttitudeTransform")
//			{								
//				PAT = new PositionAttitudeTransform();
//				if((geo_type==eGeometryTypeBox)||(geo_type==eGeometryTypeCapsule)||(geo_type==eGeometryTypeCompositeShape)
//					||(geo_type==eGeometryTypeCone)||(geo_type==eGeometryTypeCylinder)||(geo_type==eGeometryTypeHeightField)
//					||(geo_type==eGeometryTypeInfinitePlane)||(geo_type==eGeometryTypeKdTree)||(geo_type==eGeometryTypeSphere)
//					||(geo_type==eGeometryTypTriangleMesh)||(geo_type==eGeometryTypeCube)
//					)
//				{
//					if(geode!= NULL)
//					{
//						PAT->addChild(geode);
//					}
//				}
//				else if (geo_type==eGeometryTypeFile)
//				{
//					if(loadedModel!= NULL)
//					{
//						PAT->addChild(loadedModel.get());
//					}
//				}						
//				XML_Parse_PositionAttitudeTransform(spXMLNextNode, PAT);
//				grp->addChild(PAT);
//			}
//			else if(str_node_name == "Optimizer")
//			{
//				if((loadedModel!= NULL)&&(str_node_text== "T"))
//				{
//					osgUtil::Optimizer optimizer;
//					optimizer.optimize(loadedModel.get());			
//				}
//			}				
//		}
//	}	
//	if(PAT== NULL)
//	{
//		if((geo_type==eGeometryTypeBox)||(geo_type==eGeometryTypeCapsule)||(geo_type==eGeometryTypeCompositeShape)
//					||(geo_type==eGeometryTypeCone)||(geo_type==eGeometryTypeCylinder)||(geo_type==eGeometryTypeHeightField)
//					||(geo_type==eGeometryTypeInfinitePlane)||(geo_type==eGeometryTypeKdTree)||(geo_type==eGeometryTypeSphere)
//					||(geo_type==eGeometryTypTriangleMesh)||(geo_type==eGeometryTypeCube)
//					)
//		{
//			if(geode!= NULL)
//			{
//				grp->addChild(geode);
//			}
//		}
//		else if (geo_type==eGeometryTypeFile)
//		{
//			if(loadedModel!= NULL)
//			{
//				grp->addChild(loadedModel);
//			}
//		}				
//	}
//		
//}
