#ifndef DEE_VIEWER_H
#define DEE_VIEWER_H

#include "deeCore.h"

namespace dee{
	class View;

	class Viewer : public QWidget{
		
		Q_OBJECT
		
	protected:
		QVector<View*> m_views;
		vtkImageData* m_volume;

	public:
		Viewer( QWidget* a_parent=NULL );
		virtual ~Viewer();
		
		View* getView( const QString& a_id );

		void setVolume( vtkImageData* );
		vtkImageData* getVolume();

	public slots:
		void onOpenVolumeButton(); 
	};
}

#endif