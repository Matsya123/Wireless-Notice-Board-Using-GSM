import json
import boto3

dynamodb = boto3.client('dynamodb', region_name='us-east-1')
iot_client = boto3.client('iot-data', region_name='us-east-1')
sns_client = boto3.client('sns', region_name='us-east-1')  # Added SNS client

TABLE_NAME = 'TrainRespose'
TOPIC = "esp32/flight/audio"
SNS_TOPIC_ARN = "arn:aws:sns:us-east-1:654654177043:FlightAlert"  # Your SNS topic ARN

def lambda_handler(event, context):
    print("=== FULL EVENT RECEIVED ===")
    print(json.dumps(event))

    # Initialize variables early to avoid UnboundLocalError
    intent_name = "TrainStatusIntent"
    train_number = "unknown"

    try:
        # Extract intent name and slot value
        intent_name = event['sessionState']['intent']['name']
        train_number = event['sessionState']['intent']['slots']['trainNumber']['value']['interpretedValue']

        print(f"Train number received: {train_number}")

        if not train_number:
            return fail_response(intent_name, "Please provide a train number.", train_number)

        # Query DynamoDB
        response = dynamodb.get_item(
            TableName=TABLE_NAME,
            Key={'trainId': {'S': train_number}}
        )

        if 'Item' not in response:
            return fail_response(intent_name, f"Train {train_number} not found in the database.", train_number)

        mp3_id = response['Item']['mp3Id']['S']
        print(f"Retrieved mp3Id: {mp3_id}")

        # Publish to MQTT
        iot_client.publish(
            topic=TOPIC,
            qos=1,
            payload=mp3_id
        )

        

        # Success response
        return success_response(intent_name, f"Train {train_number} status is received. Ready to be announced.")


    except Exception as e:
        print(f"Error: {str(e)}")
        return fail_response(intent_name, f"Error: {str(e)}", train_number)


def success_response(intent_name, message):
    return {
        "sessionState": {
            "dialogAction": {
                "type": "Close"
            },
            "intent": {
                "name": intent_name,
                "state": "Fulfilled"
            }
        },
        "messages": [
            {
                "contentType": "PlainText",
                "content": message
            }
        ]
    }


def fail_response(intent_name, message, train_number):
    return {
        "sessionState": {
            "dialogAction": {
                "type": "Close"
            },
            "intent": {
                "name": intent_name,
                "state": "Failed"
            }
        },
        "messages": [
            {
                "contentType": "PlainText",
                "content": message
            }
        ]
    }
