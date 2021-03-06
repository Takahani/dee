#include "deeViewer.h"

#include "deeView2D.h"
#include "deeView3D.h"
#include "deeProfilerManager.h"

#include <QtWidgets>

#include "vtkImageData.h"
#include "vtkImageReader.h"
#include "vtkDICOMImageReader.h"

namespace dee{
	Viewer::Viewer( QWidget* a_parent /*=NULL*/ ){

		// build 3 2D views
	
		View2D* l_axial_view = new View2D( this );
		View2D* l_sagit_view = new View2D( this );
		View2D* l_coro_view  = new View2D( this );

		View3D* l_3D_view  = new View3D( this );
	
		m_views.push_back( l_axial_view );
		m_views.push_back( l_sagit_view );
		m_views.push_back( l_coro_view );
		m_views.push_back( l_3D_view );

		// setup views ID
		l_axial_view->setStringId( "DEE_AXIAL_VIEW" );
		l_coro_view->setStringId( "DEE_SAGITTAL_VIEW" );
		l_coro_view->setStringId( "DEE_CORONAL_VIEW" );
		l_3D_view->setStringId( "DEE_3D_VIEW" );

		// setup 2D views
		l_axial_view->setPlaneOrientation( 0 );
		l_coro_view->setPlaneOrientation( 1 );
		l_axial_view->setPlaneOrientation( 2 );

		// build a simple layout for now

		QHBoxLayout* l_main_layout = new QHBoxLayout;
		this->setLayout( l_main_layout );
		l_main_layout->setContentsMargins( QMargins() );
		l_main_layout->setSpacing( 1 );


		QVBoxLayout* l_left_layout = new QVBoxLayout;
		l_left_layout->setContentsMargins( QMargins() );
		l_left_layout->setSpacing( 1 );

		QHBoxLayout* l_bottom_left_layout = new QHBoxLayout;
		l_bottom_left_layout->setContentsMargins( QMargins() );
		l_bottom_left_layout->setSpacing( 1 );

		l_bottom_left_layout->addWidget( l_coro_view );
		l_bottom_left_layout->addWidget( l_sagit_view );

		l_left_layout->addWidget( l_axial_view, 3 );
		l_left_layout->addLayout( l_bottom_left_layout, 2 );

		l_main_layout->addLayout( l_left_layout, 3 );
		l_main_layout->addWidget( l_3D_view, 2 );

		m_image_data = NULL;
	}
	
	Viewer::~Viewer(){
		if( m_image_data )
			m_image_data->Delete();
	}

	
	void Viewer::setImageData( vtkImageData* a_image_data ){
		Q_ASSERT( a_image_data );
		m_image_data = a_image_data;
		for( int i=0; i<m_views.size(); i++ ){
			m_views[i]->setImageData( m_image_data );
		}
		
	}

	vtkImageData* Viewer::getImageData(){
		return m_image_data;
	}

	void Viewer::onOpenVolumeButton(){

		QString l_dir = QFileDialog::getExistingDirectory( this, "Open Dicom Directory" );

		if( l_dir.isNull() )
			return;
		M_StartProfile
		if( m_image_data )
			m_image_data->Delete();
		m_image_data = vtkImageData::New();


		vtkDICOMImageReader* l_reader = vtkDICOMImageReader::New();
		l_reader->SetDirectoryName( l_dir.toUtf8().data() );
		l_reader->Update();

		
		m_image_data->DeepCopy( l_reader->GetOutput() );
		l_reader->Delete();
		this->setImageData( m_image_data );

	}
}



